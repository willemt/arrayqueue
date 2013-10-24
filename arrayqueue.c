/*
 
Copyright (c) 2006, Willem-Hendrik Thiart
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * The names of its contributors may not be used to endorse or promote
      products derived from this software without specific prior written
      permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL WILLEM-HENDRIK THIART BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "arrayqueue.h"

#define INITIAL_CAPACITY 10
#define in(x) ((aqueue_in_t*)x->in)
#define ARRAYPOS(me, idx) (((byte*)me->array)[idx*in(me)->elemSize])

typedef struct
{
    int arraySize;              /* size of array */
    int count;                  /* the amount of elements in the array */
    int front, back;            /* position of the queue */
} aqueue_in_t;

/**
 * Initialize anything we need to. */
arrayqueue_t* arrayqueue_new()
{
    arrayqueue_t* me;

    me = calloc(1,sizeof(arrayqueue_t));
    me->in = calloc(1,sizeof(aqueue_in_t));
    in(me)->arraySize = INITIAL_CAPACITY;
    me->array = calloc(1,sizeof(void *) * in(me)->arraySize);
    in(me)->count = 0;
    in(me)->back = in(me)->front = 0;
    return me;
}

/**
 * Tell us if the queue is empty.
 * @return true if empty, else false */
int arrayqueue_is_empty(
    arrayqueue_t * me
)
{
    return 0 == in(me)->count;
}

static void __ensurecapacity(
    arrayqueue_t * me
)
{
    int ii, jj;
    void **temp;

    if (in(me)->count < in(me)->arraySize)
        return;

    temp = calloc(1,sizeof(void *) * in(me)->arraySize * 2);

    for (ii = 0, jj = in(me)->front; ii < in(me)->count; ii++, jj++)
    {
        if (jj == in(me)->arraySize)
            jj = 0;
        temp[ii] = me->array[jj];
    }

    /* double capacity */
    in(me)->arraySize *= 2;

    /* clean up old array */
    free(me->array);

    me->array = temp;
    in(me)->front = 0;
    in(me)->back = in(me)->count;
}

#if 0
static void __checkwrapping(
    arrayqueue_t * me
)
{
    if (in(me)->front == in(me)->arraySize)
        in(me)->front = 0;
    if (in(me)->back == in(me)->arraySize)
        in(me)->back = 0;
}
#endif

/**
 * Look at the oldest element in this queue.
 * @return the oldest element in this queue. */
void *arrayqueue_peek(
    arrayqueue_t * me
)
{
    if (arrayqueue_is_empty(me))
        return NULL;
    return ((void **) me->array)[in(me)->front];
}

void *arrayqueue_peektail(
    arrayqueue_t * me
)
{
    int pos;

    if (arrayqueue_is_empty(me))
        return NULL;

    /* don't forget about wrapping */
    pos = in(me)->back - 1;

    if (pos < 0)
        pos = in(me)->arraySize - 1;

    return ((void **) me->array)[pos];
}

/**
 * Remove oldest element from queue.
 * @return oldest element of queue */
void *arrayqueue_poll(
    arrayqueue_t * me
)
{
    const void *elem;

    if (arrayqueue_is_empty(me))
        return NULL;

//    __checkwrapping(me);
    
    elem = me->array[in(me)->front];

    in(me)->front++;
    in(me)->count--;

    return (void *) elem;
}

/**
 * Add to end of queue.
 * @param item : the value we add to the queue
 * @return 0 on success, -1 on failure */
int arrayqueue_offer(
    arrayqueue_t * me,
    void *item
)
{
    assert(item);
    assert(me);

//    __checkwrapping(me);
    __ensurecapacity(me);

    ((const void **) me->array)[in(me)->back] = item;

    in(me)->count++;
    in(me)->back++;
    return 0;
}

/**
 * Empty the queue. */
void arrayqueue_empty(
    arrayqueue_t * me
)
{
    in(me)->front = 0;
    in(me)->back = 0;
    in(me)->count = 0;
}

void arrayqueue_free(
    arrayqueue_t * me
)
{
    free(me->array);
    free(me->in);
    free(me);
}

/**
 * How many elements are there in this queue? */
int arrayqueue_count(arrayqueue_t * me)
{
    return in(me)->count;
}

void* arrayqueue_get_from_idx(arrayqueue_t * me, int idx)
{
    return me->array[(in(me)->front + idx) % in(me)->arraySize];
}

int arrayqueue_iterator_has_next(
    arrayqueue_t* me,
    arrayqueue_iterator_t* iter
)
{
    if (iter->current == in(me)->back)
    {
        return 0;
    }
    return 1;
}

void *arrayqueue_iterator_next(
    arrayqueue_t* me,
    arrayqueue_iterator_t* iter
)
{
    if (!arrayqueue_iterator_has_next(me,iter))
        return NULL;

    if (iter->current == in(me)->arraySize)
        iter->current = 0;

    return (void *) me->array[iter->current++];
}

int arrayqueue_iterator_has_next_reverse(
    arrayqueue_t* me,
    arrayqueue_iterator_t* iter
)
{
    int end;
    
    end = in(me)->front - 1;

    if (end < 0)
    {
        end = in(me)->arraySize - 1;
    }

    if (iter->current == end)
    {
        return 0;
    }

    return 1;
}

void *arrayqueue_iterator_next_reverse(
    arrayqueue_t* me,
    arrayqueue_iterator_t* iter
)
{
    void *val;
    
    if (!arrayqueue_iterator_has_next_reverse(me,iter))
        return NULL;

    val = (void *) me->array[iter->current];

    iter->current--;
    if (iter->current < 0)
    {
        iter->current = in(me)->arraySize - 1;
    }
    return val;
}

void arrayqueue_iterator_reverse(
    arrayqueue_t* me,
    arrayqueue_iterator_t* iter
)
{
    iter->current = in(me)->back - 1;
    if (iter->current < 0)
        iter->current = in(me)->arraySize - 1;
}

void arrayqueue_iterator(
    arrayqueue_t * me,
    arrayqueue_iterator_t * iter
)
{
    iter->current = in(me)->front;
}

