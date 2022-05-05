#!/usr/bin/env python3
# Driver program for C programming exercise

import argparse
import collections
import json
import os.path
import subprocess
import sys


class Tracer:

    traceDirectory = "./traces"

    traceDict = {
        1: "trace-01-ops",
        2: "trace-02-ops",
        3: "trace-03-ops",
        4: "trace-04-ops",
        5: "trace-05-ops",
        6: "trace-06-string",
        7: "trace-07-robust",
        8: "trace-08-robust",
        9: "trace-09-robust",
        10: "trace-10-malloc",
        11: "trace-11-malloc",
        12: "trace-12-malloc",
        13: "trace-13-perf",
        14: "trace-14-perf",
        15: "trace-15-perf",
    }

    traceProbs = {
        1: "Trace-01",
        2: "Trace-02",
        3: "Trace-03",
        4: "Trace-04",
        5: "Trace-05",
        6: "Trace-06",
        7: "Trace-07",
        8: "Trace-08",
        9: "Trace-09",
        10: "Trace-10",
        11: "Trace-11",
        12: "Trace-12",
        13: "Trace-13",
        14: "Trace-14",
        15: "Trace-15",
    }

    maxScores = [0, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7]

    def __init__(self, qtest, verbLevel=0, autograde=False):
        self.qtest = qtest
        self.verbLevel = verbLevel
        self.autograde = autograde

    def runTrace(self, trace_id):
        if trace_id not in self.traceDict:
            print("ERROR: No trace with id {}".format(trace_id))
            return False

        fname = os.path.join(self.traceDirectory,
                             "{}.cmd".format(self.traceDict[trace_id]))
        clist = [
            self.qtest,
            "-v", "{}".format(self.verbLevel),
            "-f", fname,
        ]

        if self.verbLevel > 0:
            print(" ".join(clist))
        sys.stdout.flush()

        try:
            retcode = subprocess.call(clist)
        except Exception as e:
            print("Call of '{}' failed: {}".format(" ".join(clist), e))
            return False

        return retcode == 0

    def run(self, tid=None):
        print("---\tTrace\t\tPoints")

        # Determine list of traces to test
        if tid is None:
            tidList = self.traceDict.keys()
        else:
            if tid not in self.traceDict:
                print("ERROR: Invalid trace ID {}".format(tid))
                return
            tidList = [tid]

        # Run each trace and collect scores
        score = 0
        maxscore = 0
        scoreDict = {k: 0 for k in self.traceDict.keys()}
        for trace_id in tidList:
            # Run the trace file
            trace_name = self.traceDict[trace_id]
            if self.verbLevel > 0:
                print()
                print("+++ TESTING trace {}:".format(trace_name))
            ok = self.runTrace(trace_id)

            # Print score of this run
            maxval = self.maxScores[trace_id]
            tval = maxval if ok else 0
            print("---\t{}\t{}/{}".format(trace_name, tval, maxval))

            # Accumulate score
            score += tval
            maxscore += maxval
            scoreDict[trace_id] = tval

        print("---\tTOTAL\t\t{}/{}".format(score, maxscore))

        if self.autograde:
            # Generate JSON string for autograder
            scores = collections.OrderedDict([
                (self.traceProbs[k], score)
                for (k, score) in scoreDict.items()
            ])
            print(json.dumps(dict(scores=scores)))


def run():
    parser = argparse.ArgumentParser(
        description='Runs the cprogramminglab driver')
    parser.add_argument('-p', metavar='PROG',
                        default='./qtest', help='Program to test')
    parser.add_argument('-t', metavar='TID',
                        type=int, help='Trace ID to test')
    parser.add_argument('-v', metavar='VLEVEL',
                        type=int, choices=[0, 1, 2, 3],
                        help='Set verbosity level (0-3)')
    parser.add_argument('-A', action='store_true', help=argparse.SUPPRESS)

    args = parser.parse_args()
    prog = args.p
    tid = args.t
    vlevel = args.v
    autograde = args.A

    if vlevel is None:
        # Default verbosity is 0 for autograde, 1 otherwise
        vlevel = 0 if autograde else 1

    t = Tracer(qtest=prog, verbLevel=vlevel, autograde=autograde)
    t.run(tid)


if __name__ == "__main__":
    run()
