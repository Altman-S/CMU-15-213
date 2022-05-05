This is the handout directory for the 15-213 C Programming Lab.

************************
Running the autograders:
************************

Before running the autograders, compile your code to create the
testing program qtest:
    linux> make

Check the correctness of your code:
    linux> make test

******
Using qtest:
******

qtest provides a command interpreter that can create and manipulate
queues.  When you execute ./qtest, it will give a command prompt
"cmd>".  Type "help" to see a list of available commands.

qtest can also test a queue implementation using a "trace file" that
gives a sequence of operations to perform.  Run ./qtest -h to see how
to run qtest with a trace file.

We've provided the trace files that will be used to test your code, in
the "traces" subdirectory.  The autograder has its own copy, which you
can't change, but you are encouraged to read the trace files, work out
what they do, and experiment with writing your own.  Many of the labs
in this course will be tested with similar trace files.  It is worth
taking some time now to get comfortable with them.

******
Files:
******

You will be modifying and handing in these two files:

queue.c                 Incomplete, buggy queue implementation.
                        You will need to modify the code in this file
                        to complete the implementation and fix the bugs.
queue.h                 Declarations for the queue implementation.
                        To fix all the bugs, you may need to modify
                        the structs in this file, for instance by
                        introducing new fields.

You should not need to modify any of the other files in this
directory.  If you do, the autograder won't use your modifications.
Please let one of the TAs or instructors know immediately if you
discover a problem requiring you to modify the other files.

The trace files are all in the traces subdirectory:

traces/trace-eg.cmd:    A simple, documented trace file to demonstrate
                        the operation of qtest.

traces/trace-XX-CAT.cmd Trace files used by driver.py to test your code.
                        We encourage you to study them to see what tests are
                        being performed.  XX is the trace number (1-15).
                        CAT describes the general nature of the test.

Finally, these files implement tools for evaluating your code.

README                  This file.
Makefile                Build logic.
driver.py               Helper script that runs qtest on a standard set
                        of traces.
qtest.c                 Main body of qtest.
console.{c,h}:          Implements the command-line interpreter for qtest
report.{c,h}:           Implements printing of information at different
                        levels of verbosity.
harness.{c,h}:          Customized version of malloc and free to provide
                        rigorous testing of your code.
