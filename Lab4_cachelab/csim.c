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


/* Global arguments (cache organization and option information) */
struct globalArgs_t {
    int help;         /* -h option */
    int vebosity;     /* -v option */
    int s;            /* -s option */
    int E;	          /* -E option */
    int b;            /* -b option */
    char* inputFile;  /* -t option */
} globalArgs;


/* Self-defined cache block */
typedef struct {
    int valid;			 /* valid bit 0/1 */
    unsigned long tag;           /* tag to match cache block */
    unsigned long time_updated;  /* last time to update for LRU */
    // char* data;       /* it's just a simple simulator, not implementat */
} cache_block;


cache_block* cache;            /* cache memory */
FILE* fp = NULL;               /* trace file */
unsigned long tag_mask = 0;    /* mask to extract tag */
unsigned long set_mask = 0;    /* mask to extract set */
unsigned long offset_mask = 0; /* mask to extrac offset */
enum { BUF_SIZE = 64 };        /* buffer for input */


/* Cache initialisation */
void cache_init() {
    /* initialise the cache memory */
    int num_lines = (1 << globalArgs.s) * globalArgs.E;
    cache = (cache_block*) malloc(num_lines * sizeof(cache_block));
    /* initialise tag_mask, set_mask and offset_mask */
    offset_mask = (1ULL << globalArgs.b) - 1;
    set_mask = ((1ULL << (globalArgs.s + globalArgs.b)) - 1) ^ offset_mask;
    tag_mask = 0xffffffffffffffff - set_mask - offset_mask;
    /* initialise the file pointer */
    fp = fopen(globalArgs.inputFile, "r");
}


/* Cache terminator */
void cache_exit() {
    free(cache);
    fclose(fp);
}


/* Get set number */
unsigned long get_set(unsigned long addr) {
    return (addr & set_mask) >> globalArgs.b;
}


/* Get tag number */
unsigned long get_tag(unsigned long addr) {
    return (addr & tag_mask) >> (globalArgs.s + globalArgs.b);
}


/* access the cache memory
 * return value:
 *   0 for hit
 *   1 for cold miss
 *   2 for eviction miss
 */
int access_mem(unsigned long addr, int type, int time) {
    unsigned long set = get_set(addr);
    unsigned long tag = get_tag(addr);
    int first_empty = -1;        /* first empty slot */
    int lru_time = 0x7fffffff;   /* last time used cache block */
    int lru_pos = -1;            /* last time used cahche block position */

    /* loop through all the blocks in the set */
    for (int i = 0; i < globalArgs.E; ++i) {
        /* cache hit */
        if (cache[set * globalArgs.E + i].tag == tag && cache[set * globalArgs.E + i].valid) {
            cache[set * globalArgs.E + i].time_updated = time;
            if (globalArgs.vebosity) {
                printf("hit ");
                /* if type is M, there must be a second ref hit */
                if (type == 'M') 
                    printf("hit\n");
                else
                    printf("\n");
            }
            return 0;
        }
        /* detect empty slot */
        else if (!cache[set * globalArgs.E + i].valid) {
            first_empty = i;
            break;
        }
        /* find lru time and position */
        else if (cache[set * globalArgs.E + i].time_updated < lru_time) {
            lru_time = cache[set * globalArgs.E + i].time_updated;
            lru_pos = i;
        }
    }

    /* cold miss */
    if (first_empty != -1) {
        cache[set * globalArgs.E + first_empty].valid = 1;
        cache[set * globalArgs.E + first_empty].tag = tag;
        cache[set * globalArgs.E + first_empty].time_updated = time;
        if (globalArgs.vebosity) {
            printf("miss ");
            /* if type is M, there must be a second ref hit */
            if (type == 'M')
                printf("hit\n");
            else
                printf("\n");
        }
        return 1;
    }
    /* capacity miss, need eviction */
    else {
        cache[set * globalArgs.E + lru_pos].tag = tag;
        cache[set * globalArgs.E + lru_pos].time_updated = time;
        if (globalArgs.vebosity) {
            printf("miss eviction ");
            /* if type is M, there must be a second ref hit */
            if (type == 'M')
                printf("hit\n");
            else
                printf("\n");
        }
        return 2;
    }
}


/* execute each instruction */
void run(int* count) {
    char buf[BUF_SIZE];    /* buffer for input */
    unsigned long addr;    /* data address */
    int type;              /* current cache access type */
    int size;              /* bytes size to access */
    int time = 0;          /* time for LRU */

    while (fgets(buf, BUF_SIZE, fp)) {
        /* ignore cache access instruction */
        if (buf[0] == 'I') {
            continue;
        }
        /* parse each line */
        int delimiter = 0;
        while (buf[delimiter] != ',') {
            ++delimiter;
        }
        buf[delimiter] = 0;
        addr = strtoul(buf + 3, NULL, 16);
        type = buf[1];
        size = atoi(buf + delimiter + 1);
        if (globalArgs.vebosity) {
            printf("%c %lx, %d", type, addr, size);
        }

        switch (access_mem(addr, type, time++)) {
            case 0:
                ++count[0];
                break;
            case 2:
                ++count[2];
                /* fall through */
            case 1:
                ++count[1];
        }

        /* if type is M, there must be a second ref hit */
        if (type == 'M') {
            ++count[0];
        }
    }
}



int main(int argc, char** argv) {
    /* initialization */
    int opt = 0;
    globalArgs.help = 0;
    globalArgs.vebosity = 0;
    globalArgs.s = -1;
    globalArgs.E = -1;
    globalArgs.b = -1;
    globalArgs.inputFile = NULL;

    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (opt) {
            case 'v':
                globalArgs.vebosity = 1;
                break;
            case 's':
                globalArgs.s = atoi(optarg);
                break;
            case 'E':
                globalArgs.E = atoi(optarg);
                break;
            case 'b':
                globalArgs.b = atoi(optarg);
                break;
            case 't':
                globalArgs.inputFile = optarg;
                break;
            case '?':
                /* Fall through */
            case 'h':
                print_usage();
                return 0;
            default:
                /* You won't actually get here. */
                break;
        }
    }

    /* check for mandatory options and constrains */
    if (globalArgs.s <= 0 || globalArgs.E <= 0 || globalArgs.b <= 0 || globalArgs.inputFile == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        print_usage();
        return 0;
    }

    int count[3] = {0};  /* hit, miss and eviction count */
    cache_init();
    run(count);
    cache_exit();
    printSummary(count[0], count[1], count[2]);
    return 0;
}

