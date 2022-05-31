/**
 * @file queue.c
 * @brief Implementation of a queue that supports FIFO and LIFO operations.
 *
 * This queue implementation uses a singly-linked list to represent the
 * queue elements. Each queue element stores a string value.
 *
 * Assignment for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Extended to store strings, 2018
 *
 * TODO: fill in your name and Andrew ID
 * @author XXX <XXX@andrew.cmu.edu>
 */

#include "queue.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief Allocates a new queue
 * @return The new queue, or NULL if memory allocation failed
 */
queue_t *queue_new(void) {
    queue_t *q = malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if (q == NULL) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/**
 * @brief Frees all memory used by a queue
 * @param[in] q The queue to free
 */
void queue_free(queue_t *q) {
    /* How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (q == NULL) {
        return;
    }
    list_ele_t *cur = q->head, *nxt = NULL;

    // free each element
    while (cur != NULL) {
        // free the string of each element
        free(cur->value);
        nxt = cur->next;
        free(cur);
        cur = nxt;
    }
    free(q);
}

/**
 * @brief Attempts to insert an element at head of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_head(queue_t *q, const char *s) {
    /* What should you do if the q is NULL? */
    if (q == NULL || s == NULL) {
        return false;
    }
    list_ele_t *newH = malloc(sizeof(list_ele_t));
    if (newH == NULL) {
        return false;
    }

    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */

    // set the value of element
    size_t numChar = 0;
    const char *cur = s;
    while (*cur != '\0') {
        numChar++;
        cur++;
    }
    char *newValue = malloc((numChar + 1) * sizeof(char));
    if (newValue == NULL) {
        free(newH);
        return false;
    }
    const char *source = s;
    char *destination = newValue;
    for (size_t i = 0; i < numChar + 1; i++) {
        *(destination + i) = *(source + i);
    }
    newH->value = newValue;

    // insert into the head
    if (q->head == NULL) {
        q->tail = newH;
    }
    newH->next = q->head;
    q->head = newH;
    q->size += 1;
    return true;
}

/**
 * @brief Attempts to insert an element at tail of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_tail(queue_t *q, const char *s) {
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL || s == NULL) {
        return false;
    }
    list_ele_t *newT = malloc(sizeof(list_ele_t));
    if (newT == NULL) {
        return false;
    }

    // set the value of element
    size_t numChar = 0;
    const char *cur = s;
    while (*cur != '\0') {
        numChar++;
        cur++;
    }
    char *newValue = malloc((numChar + 1) * sizeof(char));
    if (newValue == NULL) {
        free(newT);
        return false;
    }
    const char *source = s;
    char *destination = newValue;
    for (size_t i = 0; i < numChar + 1; i++) {
        *(destination + i) = *(source + i);
    }
    newT->value = newValue;
    newT->next = NULL;

    // insert into the tail
    if (q->head == NULL) {
        q->head = newT;
        q->tail = newT;
        q->size = 1;
    } else {
        q->tail->next = newT;
        q->tail = newT;
        q->size += 1;
    }

    return true;
}

/**
 * @brief Attempts to remove an element from head of a queue
 *
 * If removal succeeds, this function frees all memory used by the
 * removed list element and its string value before returning.
 *
 * If removal succeeds and `buf` is non-NULL, this function copies up to
 * `bufsize - 1` characters from the removed string into `buf`, and writes
 * a null terminator '\0' after the copied string.
 *
 * @param[in]  q       The queue to remove from
 * @param[out] buf     Output buffer to write a string value into
 * @param[in]  bufsize Size of the buffer `buf` points to
 *
 * @return true if removal succeeded
 * @return false if q is NULL or empty
 */
bool queue_remove_head(queue_t *q, char *buf, size_t bufsize) {
    /* You need to fix up this code. */
    if (q == NULL || q->head == NULL) {
        return false;
    }

    list_ele_t *rmHead = q->head, *newHead = q->head->next;

    // copy characters into 'buf'
    if (buf != NULL) {
        char *source = rmHead->value, *destination = buf;
        for (size_t i = 0; i < bufsize - 1 && *source != '\0'; ++i) {
            *destination = *source;
            destination++;
            source++;
        }
        *destination = '\0';
    }

    // free all memory of the element
    free(rmHead->value);
    free(rmHead);

    q->head = newHead;
    q->size -= 1;
    if (rmHead == q->tail) {
        q->tail = NULL;
    }

    return true;
}

/**
 * @brief Returns the number of elements in a queue
 *
 * This function runs in O(1) time.
 *
 * @param[in] q The queue to examine
 *
 * @return the number of elements in the queue, or
 *         0 if q is NULL or empty
 */
size_t queue_size(queue_t *q) {
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL || q->head == NULL) {
        return 0;
    }

    return q->size;
}

/**
 * @brief Reverse the elements in a queue
 *
 * This function does not allocate or free any list elements, i.e. it does
 * not call malloc or free, including inside helper functions. Instead, it
 * rearranges the existing elements of the queue.
 *
 * @param[in] q The queue to reverse
 */
void queue_reverse(queue_t *q) {
    /* You need to write the code for this function */
    if (q == NULL || q->head == NULL || q->head->next == NULL) {
        return;
    }

    list_ele_t *pre = q->head;
    list_ele_t *cur = q->head->next;
    list_ele_t *nxt = q->head->next->next;
    list_ele_t *oldHead = q->head, *oldTail = q->tail;

    pre->next = NULL;
    cur->next = pre;
    while (nxt != NULL) {
        pre = cur;
        cur = nxt;
        nxt = nxt->next;
        cur->next = pre;
    }

    q->head = oldTail;
    q->tail = oldHead;
}
