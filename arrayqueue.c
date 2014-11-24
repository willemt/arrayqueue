
/**
 * Copyright (c) 2014, Willem-Hendrik Thiart
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "arrayqueue.h"

void aqueuenp_init(arrayqueuenp_t* me, size_t size, size_t m_size)
{
    me->m_size = m_size;
    me->size = size;
    me->count = 0;
    me->back = me->front = 0;
}

arrayqueuenp_t* aqueuenp_new(size_t size, size_t m_size)
{
    arrayqueuenp_t *me = malloc(aqueuenp_sizeof(size, m_size));

    if (!me)
        return NULL;

    aqueuenp_init(me, size, m_size);

    return me;
}

size_t aqueuenp_sizeof(size_t size, size_t m_size)
{
    return sizeof(arrayqueuenp_t) + size * m_size;
}

int aqueuenp_is_empty(const arrayqueuenp_t * me)
{
    return 0 == me->count;
}

int aqueuenp_is_full(arrayqueuenp_t * me)
{
    return me->size == me->count;
}

static arrayqueuenp_t* __ensurecapacity(arrayqueuenp_t * me)
{
    if (me->count < me->size)
        return me;

    arrayqueuenp_t *new = malloc(aqueuenp_sizeof(me->size * 2, me->m_size));

    size_t ii, jj;
    for (ii = 0, jj = me->front; ii < me->count; ii++, jj++)
    {
        if (jj == me->size)
            jj = 0;
        memcpy(new->array + ii * me->m_size, me->array + jj * me->m_size,
               me->m_size);
    }

    new->m_size = me->m_size;
    new->size = me->size * 2;
    new->front = 0;
    new->count = new->back = me->count;
    free(me);
    return new;
}

void *aqueuenp_peek(arrayqueuenp_t * me)
{
    if (aqueuenp_is_empty(me))
        return NULL;
    return me->array + me->front * me->m_size;
}

int aqueuenp_poll(arrayqueuenp_t * me)
{
    if (aqueuenp_is_empty(me))
        return -1;

    me->front++;
    if (me->size == me->front)
        me->front = 0;
    me->count--;
    return 0;
}

int aqueuenp_offerensure(arrayqueuenp_t **me_ptr, void *item)
{
    if (NULL == (*me_ptr = __ensurecapacity(*me_ptr)))
        return -1;

    arrayqueuenp_t* me = *me_ptr;

    memcpy(me->array + me->back * me->m_size, item, me->m_size);
    me->count++;
    me->back++;

    if (!aqueuenp_is_full(me) && me->size == me->back)
        me->back = 0;
    return 0;
}

int aqueuenp_offer(arrayqueuenp_t *me, void *item)
{
    if (aqueuenp_is_full(me))
        return -1;

    memcpy(me->array + me->back * me->m_size, item, me->m_size);
    me->count++;
    me->back++;

    if (me->size == me->back)
        me->back = 0;
    return 0;
}

void aqueuenp_empty(arrayqueuenp_t * me)
{
    me->front = me->back = me->count = 0;
}

void aqueuenp_free(arrayqueuenp_t * me)
{
    free(me);
}

int aqueuenp_count(const arrayqueuenp_t * me)
{
    return me->count;
}

int aqueuenp_size(const arrayqueuenp_t * me)
{
    return me->count;
}
