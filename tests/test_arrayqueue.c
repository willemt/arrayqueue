
#include <stdbool.h>
#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CuTest.h"

#include "arrayqueue.h"

void TestarrayQueue_new_is_empty(
    CuTest * tc
)
{
    void *qu = arrayqueue_new(10);
    CuAssertTrue(tc, arrayqueue_is_empty(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_offer_adds_new_item(
    CuTest * tc
)
{
    char *item = "testitem";
    void *qu = arrayqueue_new(10);

    arrayqueue_offer(qu, item);
    CuAssertTrue(tc, 1 == arrayqueue_count(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_cannot_offer_over_capacity(
    CuTest * tc
)
{
    char *item = "testitem";
    void *qu = arrayqueue_new(1);

    arrayqueue_offer(qu, item);
    CuAssertTrue(tc, -1 == arrayqueue_offer(qu, item));
    CuAssertTrue(tc, 1 == arrayqueue_count(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_peek_gets_head(
    CuTest * tc
)
{
    char *item = "testitem";
    char *item2 = "testitem2";
    void *qu = arrayqueue_new(10);

    arrayqueue_offer(qu, item);
    arrayqueue_offer(qu, item2);
    CuAssertTrue(tc, item == arrayqueue_peek(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_peek_tail_gets_tail(
    CuTest * tc
)
{
    char *item = "testitem";
    char *item2 = "testitem2";
    void *qu = arrayqueue_new(10); 

    arrayqueue_offer(qu, item);
    arrayqueue_offer(qu, item2);
    CuAssertTrue(tc, item2 == arrayqueue_peektail(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_poll_tail(
    CuTest * tc
)
{
    char *item = "testitem";
    char *item2 = "testitem2";
    void *qu = arrayqueue_new(10);

    arrayqueue_offer(qu, item);
    arrayqueue_offer(qu, item2);
    CuAssertTrue(tc, item2 == arrayqueue_polltail(qu));
    CuAssertTrue(tc, 1 == arrayqueue_count(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_empty_empties_queue(
    CuTest * tc
)
{
    char *item = "testitem";
    void *qu = arrayqueue_new(10);

    arrayqueue_offer(qu, item);
    arrayqueue_empty(qu);
    CuAssertTrue(tc, 0 == arrayqueue_count(qu));
    CuAssertTrue(tc, arrayqueue_is_empty(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_cant_poll_with_no_contents(
    CuTest * tc
)
{
    char *item = "testitem";
    void *qu = arrayqueue_new(10);
    arrayqueue_offer(qu, item);
    CuAssertTrue(tc, item == arrayqueue_poll(qu));
    CuAssertTrue(tc, 0 == arrayqueue_count(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_offer_and_poll_item(
    CuTest * tc
)
{
    char *item = "testitem";
    void *qu = arrayqueue_new(10);

    arrayqueue_offer(qu, item);
    CuAssertTrue(tc, item == arrayqueue_poll(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_fifo(
    CuTest * tc
)
{
    char *item = "testitem", *item2 = "testitem2";
    void *qu = arrayqueue_new(10);

    arrayqueue_offer(qu, item);
    arrayqueue_offer(qu, item2);
    CuAssertTrue(tc, item == arrayqueue_poll(qu));
    CuAssertTrue(tc, item2 == arrayqueue_poll(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_poll_offer_past_boundary(
    CuTest * tc
)
{
    char *item1 = "testitem1";
    char *item2 = "testitem2";
    char *item3 = "testitem3";
    void *qu = arrayqueue_new(2);

    arrayqueue_offer(qu, item1);
    arrayqueue_offer(qu, item2);
    CuAssertTrue(tc, item1 == arrayqueue_poll(qu));
    arrayqueue_offer(qu, item3);
    CuAssertTrue(tc, item2 == arrayqueue_poll(qu));
    CuAssertTrue(tc, item3 == arrayqueue_poll(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_get_item_at_idx(
    CuTest * tc
)
{
    char *item = "testitem", *item2 = "testitem2";
    void *qu = arrayqueue_new(10);

    arrayqueue_offer(qu, item);
    arrayqueue_offer(qu, item2);
    CuAssertTrue(tc, item == arrayqueue_get_from_idx(qu,0));
    CuAssertTrue(tc, item2 == arrayqueue_get_from_idx(qu,1));
    arrayqueue_free(qu);
}

void TestarrayQueue_get_item_at_idx_after_poll(
    CuTest * tc
)
{
    char *item = "testitem", *item2 = "testitem2";
    void *qu = arrayqueue_new(10);

    arrayqueue_offer(qu, item);
    arrayqueue_offer(qu, item2);
    arrayqueue_poll(qu);
    CuAssertTrue(tc, item2 == arrayqueue_get_from_idx(qu,0));
    arrayqueue_free(qu);
}

