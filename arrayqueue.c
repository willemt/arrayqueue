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

arrayqueue_t* arrayqueue_new(unsigned int size)
{
    arrayqueue_t* me = calloc(1,sizeof(arrayqueue_t));
    me->array = calloc(1,sizeof(void *) * size);
    me->size = size;
    me->count = 0;
    me->back = me->front = 0;
    return me;
}

int arrayqueue_is_empty(arrayqueue_t * me)
{
    return 0 == me->count;
}

void arrayqueue_ensurecapacity(arrayqueue_t * me)
{
    if (me->count < me->size)
        return;

    void **temp = calloc(1,sizeof(void *) * me->size * 2);

    int ii, jj;
    for (ii = 0, jj = me->front; ii < me->count; ii++, jj++)
    {
        if (jj == me->size)
            jj = 0;
        temp[ii] = me->array[jj];
    }

    me->size *= 2;

    free(me->array);

    me->array = temp;
    me->front = 0;
    me->back = me->count;
}

void *arrayqueue_peek(arrayqueue_t * me)
{
    if (arrayqueue_is_empty(me))
        return NULL;
    return ((void **) me->array)[me->front];
}

void *arrayqueue_peektail(arrayqueue_t * me)
{
    if (arrayqueue_is_empty(me))
        return NULL;

    int pos = me->back - 1;

    /* don't forget about wrapping */
    if (pos < 0)
        pos = me->size - 1;

    return ((void **) me->array)[pos];
}

void *arrayqueue_poll(arrayqueue_t * me)
{
    if (arrayqueue_is_empty(me))
        return NULL;

    const void *elem = me->array[me->front];

    me->front++;
    me->count--;

    return (void *) elem;
}

void *arrayqueue_polltail(arrayqueue_t * me)
{
    if (arrayqueue_is_empty(me))
        return NULL;

    me->back--;
    me->count--;
    if (-1 == me->back)
        me->back = me->size;

    return me->array[me->back];
}

int arrayqueue_offer(arrayqueue_t * me, void *item)
{
    if (me->count == me->size)
        return -1;
    ((const void **) me->array)[me->back] = item;
    me->count++;
    me->back++;
    return 0;
}

void arrayqueue_empty(arrayqueue_t * me)
{
    me->front = me->back = me->count = 0;
}

void arrayqueue_free(arrayqueue_t * me)
{
    free(me->array);
    free(me);
}

int arrayqueue_count(arrayqueue_t * me)
{
    return me->count;
}

void* arrayqueue_get_from_idx(arrayqueue_t * me, int idx)
{
    return me->array[(me->front + idx) % me->size];
}

int arrayqueue_iterator_has_next(arrayqueue_t* me, arrayqueue_iterator_t* iter)
{
    if (iter->current == me->back)
    {
        return 0;
    }
    return 1;
}

void *arrayqueue_iterator_next(arrayqueue_t* me, arrayqueue_iterator_t* iter)
{
    if (!arrayqueue_iterator_has_next(me,iter))
        return NULL;

    if (iter->current == me->size)
        iter->current = 0;

    return (void *) me->array[iter->current++];
}

int arrayqueue_iterator_has_next_reverse(arrayqueue_t* me, arrayqueue_iterator_t* iter)
{
    int end = me->front - 1;

    if (end < 0)
    {
        end = me->size - 1;
    }

    if (iter->current == end)
    {
        return 0;
    }

    return 1;
}

void *arrayqueue_iterator_next_reverse(arrayqueue_t* me, arrayqueue_iterator_t* iter)
{
    if (!arrayqueue_iterator_has_next_reverse(me,iter))
        return NULL;

    void *val = (void *) me->array[iter->current];

    iter->current--;
    if (iter->current < 0)
    {
        iter->current = me->size - 1;
    }
    return val;
}

void arrayqueue_iterator_reverse(arrayqueue_t* me, arrayqueue_iterator_t* iter)
{
    iter->current = me->back - 1;
    if (iter->current < 0)
        iter->current = me->size - 1;
}

void arrayqueue_iterator(arrayqueue_t * me, arrayqueue_iterator_t * iter)
{
    iter->current = me->front;
}

