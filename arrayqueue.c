
/**
 * Copyright (c) 2014, Willem-Hendrik Thiart
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "arrayqueue.h"

arrayqueue_t* arrayqueue_new(unsigned int size)
{
    arrayqueue_t* me = calloc(1, sizeof(arrayqueue_t));
    me->array = calloc(1, sizeof(void *) * size);
    me->size = size;
    me->count = 0;
    me->back = me->front = 0;
    return me;
}

int arrayqueue_is_empty(arrayqueue_t * me)
{
    return 0 == me->count;
}

int arrayqueue_is_full(arrayqueue_t * me)
{
    return me->size == me->count;
}

void arrayqueue_ensurecapacity(arrayqueue_t * me)
{
    if (me->count < me->size)
        return;

    void **temp = calloc(1, sizeof(void *) * me->size * 2);

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
    return ((void**)me->array)[me->front];
}

void *arrayqueue_peektail(arrayqueue_t * me)
{
    if (arrayqueue_is_empty(me))
        return NULL;

    int pos = me->back - 1;

    /* don't forget about wrapping */
    if (pos < 0)
        pos = me->size - 1;

    return ((void**)me->array)[pos];
}

void *arrayqueue_poll(arrayqueue_t * me)
{
    if (arrayqueue_is_empty(me))
        return NULL;

    void *elem = me->array[me->front];

    me->front++;
    if (me->size == me->front)
        me->front = 0;
    me->count--;

    return elem;
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
    if (arrayqueue_is_full(me))
        return -1;
    ((const void**)me->array)[me->back] = item;
    me->count++;
    me->back++;

    if (me->size == me->back)
        me->back = 0;
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
        return 0;
    return 1;
}

void *arrayqueue_iterator_next(arrayqueue_t* me, arrayqueue_iterator_t* iter)
{
    if (!arrayqueue_iterator_has_next(me, iter))
        return NULL;

    if (iter->current == me->size)
        iter->current = 0;

    return (void*)me->array[iter->current++];
}

int arrayqueue_iterator_has_next_reverse(arrayqueue_t* me,
                                         arrayqueue_iterator_t* iter)
{
    int end = me->front - 1;

    if (end < 0)
        end = me->size - 1;

    if (iter->current == end)
        return 0;

    return 1;
}

void *arrayqueue_iterator_next_reverse(arrayqueue_t* me,
                                       arrayqueue_iterator_t* iter)
{
    if (!arrayqueue_iterator_has_next_reverse(me, iter))
        return NULL;

    void *val = (void*)me->array[iter->current];

    iter->current--;
    if (iter->current < 0)
        iter->current = me->size - 1;
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
