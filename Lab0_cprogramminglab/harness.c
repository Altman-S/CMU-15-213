/* Test support code */

#define _XOPEN_SOURCE 700
/* Our program needs to use regular malloc/free */
#define INTERNAL 1

#include "harness.h"
#include "report.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* A few functions in this file intentionally don't use their
   arguments.  */
#define UNUSED __attribute__((unused))

/** Special values **/

/* Byte to fill newly malloced space with */
#define FILLCHAR 0x55

/** Data structures used by our code **/

static size_t allocated_count = 0;
/* Percent probability of malloc failure */
int fail_probability = 0;
static bool cautious_mode = true;
static bool noallocate_mode = false;
static bool error_occurred = false;
static const unsigned int time_limit = 1;

/*
  Internal functions
 */
/* Should this allocation fail? */
static bool fail_allocation(void) {
    double weight = (double)rand() / RAND_MAX;
    return (weight < 0.01 * fail_probability);
}

/*
  Implementation of application functions
 */
void *test_malloc(size_t size) {
    if (noallocate_mode) {
        report_event(MSG_FATAL, "Calls to malloc disallowed");
        return NULL;
    }
    if (fail_allocation()) {
        report_event(MSG_WARN, "Malloc returning NULL");
        return NULL;
    }

    void *p = malloc(size);
    if (p == NULL) {
        report_event(MSG_FATAL, "Couldn't allocate any more memory");
        error_occurred = true;
        return p;
    }
    memset(p, FILLCHAR, size);
    allocated_count++;
    return p;
}

void *test_calloc(size_t num, size_t size) {
    if (num > SIZE_MAX / size) {
        return NULL;
    }
    void *mem = test_malloc(num * size);
    if (mem != NULL) {
        memset(mem, 0, num * size);
    }
    return mem;
}

void *test_realloc(void *ptr UNUSED, size_t size UNUSED) {
    report_event(MSG_FATAL, "Calls to realloc disallowed");
    return NULL;
}

void test_free(void *p) {
    if (noallocate_mode) {
        report_event(MSG_FATAL, "Calls to free disallowed");
        return;
    }
    if (p == NULL) {
        return;
    }

    free(p);
    allocated_count--;
}

size_t allocation_check(void) {
    return allocated_count;
}

/*
  Implementation of functions for testing
 */

/*
  Set/unset cautious mode.
  In this mode, makes extra sure any block to be freed is currently allocated.
*/
void set_cautious_mode(bool cautious) {
    cautious_mode = cautious;
}

/*
  Set/unset restricted allocation mode.
  In this mode, calls to malloc and free are disallowed.
 */
void set_noallocate_mode(bool noallocate) {
    noallocate_mode = noallocate;
}

/*
  Return whether any errors have occurred since last time set error limit
 */
bool error_check(void) {
    bool e = error_occurred;
    error_occurred = false;
    return e;
}

/*
 * Arm a timeout for a tested operation.
 */
void arm_timeout(void) {
    alarm(time_limit);
}

/*
 * Cancel a running timeout after the tested operation is completed.
 */
void cancel_timeout(void) {
    alarm(0);
}
