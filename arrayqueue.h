#ifndef ARRAYQUEUENP_H
#define ARRAYQUEUENP_H

typedef struct
{
    size_t m_size;      /* size of member */
    size_t size;        /* size of array */
    size_t count;       /* the amount of items in the array */
    size_t front, back; /* position of the queue */
    void *array[];
} arrayqueuenp_t;

typedef struct
{
    int current;
} arrayqueue_iter_t;

/**
 * Create a new data structure and initialise it
 *
 * malloc()s space
 *
 * @param[in] size Initial size of queue
 * @return initialised queue */
arrayqueuenp_t* aqueuenp_new(size_t size, size_t m_size);

/**
 * Create a new data structure and initialise it.
 *
 * No malloc()s are performed.
 *
 * @param[in] size Initial size of queue
 * @return initialised queue */
void aqueuenp_init(arrayqueuenp_t* qu, size_t size, size_t m_size);

/**
 * @return number of bytes needed for a queue of this size. */
size_t aqueuenp_sizeof(size_t size, size_t m_size);

/**
 * Is the queue empty?
 *
 * @return 1 if empty; otherwise 0 */
int aqueuenp_is_empty(const arrayqueuenp_t * qu);

/**
 * @return oldest item in this queue. */
void *aqueuenp_peek(arrayqueuenp_t * qu);

/**
 * Remove oldest item from queue.
 *
 * @return 0 on sucess; -1 on failure */
int aqueuenp_poll(arrayqueuenp_t * qu);

/**
 * Add item
 *
 * Ensures that the queue can hold the item.
 *
 * NOTE:
 *  malloc() possibly called.
 *  The queue pointer will be changed if the queu needs to be enlarged.
 *
 * @param[in/out] qu_ptr Pointer to the queue. Changed when queue is enlarged.
 * @param[in] item The item to be added
 * @return 0 on success; -1 on failure */
int aqueuenp_offerensure(arrayqueuenp_t ** qu_ptr, void *item);

/**
 * Add item
 *
 * An error will occur if there isn't enough space for this item.
 *
 * NOTE:
 *  no malloc()s called.
 *
 * @param[in] item The item to be added
 * @return 0 on success; -1 on error */
int aqueuenp_offer(arrayqueuenp_t * qu, void *item);

/**
 * Empty the queue */
void aqueuenp_empty(arrayqueuenp_t * qu);

void aqueuenp_free(arrayqueuenp_t * qu);

/**
 * @return number of items */
int aqueuenp_count(const arrayqueuenp_t * qu);

int aqueuenp_size(const arrayqueuenp_t * qu);

int aqueuenp_iter_has_next(arrayqueuenp_t* qu, arrayqueue_iter_t* iter);

void *aqueuenp_iter_next(arrayqueuenp_t* qu, arrayqueue_iter_t* iter);

int aqueuenp_iter_has_next_reverse(arrayqueuenp_t* qu, arrayqueue_iter_t* iter);

void *aqueuenp_iter_next_reverse(arrayqueuenp_t* qu, arrayqueue_iter_t* iter);

void aqueuenp_iter_reverse(arrayqueuenp_t* qu, arrayqueue_iter_t* iter);

void aqueuenp_iter(arrayqueuenp_t * qu, arrayqueue_iter_t * iter);

#endif /* ARRAYQUEUENP_H */
