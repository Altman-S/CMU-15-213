/*
 * csim.c - cache simulator
 * 
 * Pi
 * April 17th 2023
 */

#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


/* print the usage for the cache simulator */
void print_usage() {
    puts(
        "Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n"
        "Options:\n"
        "  -h: Optional help flag that prints usage info\n"
        "  -v: Optional verbose flag that displays trace info\n"
        "  -s <s>: Number of set index bits (S = 2^s is the number of sets)\n"
        "  -E <E>: Associativity (number of lines per set)\n"
        "  -b <b>: Number of block bits (B = 2^b is the block size)\n"
        "  -t <tracefile>: Name of the valgrind trace to replay\n"
        "\n"
        "Examples:\n"
        "  linux> ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n"
        "  linux> ./csim-ref -v -s 4 -E 1 -b 4 -t traces/yi.trace"
    );
}


/* global arguments (cache organization and option information) */
struct globalArgs_t {
    int help;         /* -h option */
    int vebosity;     /* -v option */
    int s;            /* -s option */
    int E;	      /* -E option */
    int b;            /* -b option */
    char* inputFile;  /* -t option */
} globalArgs;


/* self-defined cache block */
typedef struct {
    int valid;			 /* valid bit 0/1 */
    unsigned long tag;           /* tag to match cache block */
    unsigned long time_updated;  /* last time to update for LRU */
    // char* data;
} cache_block;


/* cache initialisation */
void cache_init() {

}


/* cache terminator */
void cache_exit() {

}


/* assess the memory
 * return value:
 *   0 for hit
 *   1 for miss
 *   2 for eviction
 */
int assess_mem() {

}


/* execute each instruction */
void run() {

}



int main()
{
    printSummary(0, 0, 0);
    return 0;
}
