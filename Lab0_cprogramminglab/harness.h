#ifndef HARNESS_H
#define HARNESS_H

#include <stddef.h>

/*
  This test harness enables us to do stringent testing of code.
  It overloads the library versions of malloc and free with ones that
  allow checking for common allocation errors.
*/

void *test_malloc(size_t size);
void *test_calloc(size_t num, size_t size);
void test_free(void *p);

#ifdef INTERNAL
#include <stdbool.h>

/* Report number of allocated blocks */
size_t allocation_check(void);

/* Probability of malloc failing, expressed as percent */
extern int fail_probability;

/*
  Set/unset cautious mode.
  In this mode, makes extra sure any block to be freed is currently allocated.
*/
void set_cautious_mode(bool cautious);

/*
  Set/unset restricted allocation mode.
  In this mode, calls to malloc and free are disallowed.
 */
void set_noallocate_mode(bool noallocate);

/*
  Return whether any errors have occurred since last time checked
 */
bool error_check(void);

/*
 * Arm a timeout for a tested operation.
 */
void arm_timeout(void);

/*
 * Cancel a running timeout after the tested operation is completed.
 */
void cancel_timeout(void);

#else
/* Tested program use our versions of malloc and free */
#define malloc test_malloc
#define calloc test_calloc
#define realloc test_realloc
#define free test_free
#endif

/* Hack so that old solutions continue working */
#define q_new queue_new
#define q_free queue_free
#define q_insert_head queue_insert_head
#define q_insert_tail queue_insert_tail
#define q_remove_head queue_remove_head
#define q_size queue_size
#define q_reverse queue_reverse

#endif /* HARNESS_H */
