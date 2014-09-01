#ifndef ARRAYQUEUE_H
#define ARRAYQUEUE_H

typedef struct {
    void **array;
    int size;              /* size of array */
    int count;             /* the amount of elements in the array */
    int front, back;       /* position of the queue */
} arrayqueue_t;

typedef struct
{
    int current;
} arrayqueue_iterator_t;

/**
 * Initialize anything we need to. */
arrayqueue_t* arrayqueue_new(unsigned int size);

/**
 * Tell us if the queue is empty.
 * @return true if empty, else false */
int arrayqueue_is_empty(arrayqueue_t * qu);

/**
 * @return the oldest element in this queue. */
void *arrayqueue_peek(arrayqueue_t * qu);

void *arrayqueue_peektail(arrayqueue_t * qu);

/**
 * Remove youngest element from queue.
 * @return youngest element of queue */
void *arrayqueue_polltail(arrayqueue_t * qu);

/**
 * Remove oldest element from queue.
 * @return oldest element of queue */
void *arrayqueue_poll(arrayqueue_t * qu);

/**
 * Add to end of queue.
 * @param[in] item the value we add to the queue
 * @return 0 on success, -1 on failure */
int arrayqueue_offer(arrayqueue_t * qu, void *item);

/**
 * Empty the queue. */
void arrayqueue_empty(arrayqueue_t * qu);

void arrayqueue_free(arrayqueue_t * qu);

/**
 * @return number of items this queue */
int arrayqueue_count(arrayqueue_t * qu);

int arrayqueue_iterator_has_next(arrayqueue_t* qu, arrayqueue_iterator_t* iter);

void *arrayqueue_iterator_next(arrayqueue_t* qu, arrayqueue_iterator_t* iter);

int arrayqueue_iterator_has_next_reverse(arrayqueue_t* qu, arrayqueue_iterator_t* iter);

void *arrayqueue_iterator_next_reverse(arrayqueue_t* qu, arrayqueue_iterator_t* iter);

void arrayqueue_iterator_reverse(arrayqueue_t* qu, arrayqueue_iterator_t* iter);

void arrayqueue_iterator(arrayqueue_t * qu, arrayqueue_iterator_t * iter);

void* arrayqueue_get_from_idx(arrayqueue_t * me, int idx);

#endif /* ARRAYQUEUE_H */
