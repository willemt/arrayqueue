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
#include <stdbool.h>
#include <assert.h>
#include "arrayqueue.h"

#define INITIAL_CAPACITY 10
#define in(x) ((aqueue_in_t*)x->in)
#define ARRAYPOS(qu, idx) (((byte*)qu->array)[idx*in(qu)->elemSize])

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
    arrayqueue_t* qu;

    qu = calloc(1,sizeof(arrayqueue_t));
    qu->in = calloc(1,sizeof(aqueue_in_t));
    in(qu)->arraySize = INITIAL_CAPACITY;
    qu->array = calloc(1,sizeof(void *) * in(qu)->arraySize);
    in(qu)->count = 0;
    in(qu)->back = in(qu)->front = 0;
    return qu;
}

/**
 * Tell us if the queue is empty.
 * @return true if empty, else false */
bool arrayqueue_is_empty(
    arrayqueue_t * qu
)
{
    return 0 == in(qu)->count;
}

static void __ensurecapacity(
    arrayqueue_t * qu
)
{
    int ii, jj;
    const void **temp;

    if (in(qu)->count < in(qu)->arraySize)
        return;

    temp = calloc(1,sizeof(void *) * in(qu)->arraySize * 2);

    for (ii = 0, jj = in(qu)->front; ii < in(qu)->count; ii++, jj++)
    {
        if (jj == in(qu)->arraySize)
            jj = 0;
        temp[ii] = qu->array[jj];
    }

    /* double capacity */
    in(qu)->arraySize *= 2;

    /* clean up old array */
    free(qu->array);

    qu->array = temp;
    in(qu)->front = 0;
    in(qu)->back = in(qu)->count;
}

#if 0
static void __checkwrapping(
    arrayqueue_t * qu
)
{
    if (in(qu)->front == in(qu)->arraySize)
        in(qu)->front = 0;
    if (in(qu)->back == in(qu)->arraySize)
        in(qu)->back = 0;
}
#endif

/**
 * Look at the oldest element in this queue.
 * @return the oldest element in this queue. */
void *arrayqueue_peek(
    arrayqueue_t * qu
)
{
    if (arrayqueue_is_empty(qu))
        return NULL;
    return ((void **) qu->array)[in(qu)->front];
}

void *arrayqueue_peektail(
    arrayqueue_t * qu
)
{
    int pos;

    if (arrayqueue_is_empty(qu))
        return NULL;

    /* don't forget about wrapping */
    pos = in(qu)->back - 1;

    if (pos < 0)
        pos = in(qu)->arraySize - 1;

    return ((void **) qu->array)[pos];
}

/**
 * Remove oldest element from queue.
 * @return oldest element of queue */
void *arrayqueue_poll(
    arrayqueue_t * qu
)
{
    const void *elem;

    if (arrayqueue_is_empty(qu))
        return NULL;

//    __checkwrapping(qu);
    
    elem = qu->array[in(qu)->front];

    in(qu)->front++;
    in(qu)->count--;

    return (void *) elem;
}

/**
 * Add to end of queue.
 * @param item : the value we add to the queue
 * @return 0 on success, -1 on failure */
int arrayqueue_offer(
    arrayqueue_t * qu,
    void *item
)
{
    assert(item);
    assert(qu);

//    __checkwrapping(qu);
    __ensurecapacity(qu);

    ((const void **) qu->array)[in(qu)->back] = item;

    in(qu)->count++;
    in(qu)->back++;
    return 0;
}

/**
 * Empty the queue. */
void arrayqueue_empty(
    arrayqueue_t * qu
)
{
    in(qu)->front = 0;
    in(qu)->back = 0;
    in(qu)->count = 0;
}

void arrayqueue_free(
    arrayqueue_t * qu
)
{
    free(qu->array);
    free(qu->in);
    free(qu);
}

/**
 * How many elements are there in this queue? */
int arrayqueue_count(
    arrayqueue_t * qu
)
{
    return in(qu)->count;
}
