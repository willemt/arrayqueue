
#include <stdbool.h>
#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CuTest.h"

#include "arrayqueue.h"

void TestarrayQueue_NewIsEmpty(
    CuTest * tc
)
{
    void *qu;

    qu = arrayqueue_new();
    CuAssertTrue(tc, arrayqueue_is_empty(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_offer(
    CuTest * tc
)
{
    void *qu;

    char *item = "testitem";

    qu = arrayqueue_new();

    arrayqueue_offer(qu, item);
    CuAssertTrue(tc, 1 == arrayqueue_count(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_peekGetsHead(
    CuTest * tc
)
{
    void *qu;
    char *item = "testitem";
    char *item2 = "testitem2";

    qu = arrayqueue_new();

    arrayqueue_offer(qu, item);
    arrayqueue_offer(qu, item2);
    CuAssertTrue(tc, item == arrayqueue_peek(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_peekTailGetsTail(
    CuTest * tc
)
{
    void *qu;
    char *item = "testitem";
    char *item2 = "testitem2";

    qu = arrayqueue_new();

    arrayqueue_offer(qu, item);
    arrayqueue_offer(qu, item2);
    CuAssertTrue(tc, item2 == arrayqueue_peektail(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_EmptyEmptiesQueue(
    CuTest * tc
)
{
    void *qu;

    char *item = "testitem";

    qu = arrayqueue_new();

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
    void *qu;

    char *item = "testitem";

    qu = arrayqueue_new();
    arrayqueue_offer(qu, item);
    CuAssertTrue(tc, item == arrayqueue_poll(qu));
    CuAssertTrue(tc, 0 == arrayqueue_count(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_offer_and_poll_item(
    CuTest * tc
)
{
    void *qu;

    char *item = "testitem";

    qu = arrayqueue_new();

    arrayqueue_offer(qu, item);
    CuAssertTrue(tc, item == arrayqueue_poll(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_fifo(
    CuTest * tc
)
{
    void *qu;

    char *item = "testitem", *item2 = "testitem2";

    qu = arrayqueue_new();

    arrayqueue_offer(qu, item);
    arrayqueue_offer(qu, item2);
    CuAssertTrue(tc, item == arrayqueue_poll(qu));
    CuAssertTrue(tc, item2 == arrayqueue_poll(qu));
    arrayqueue_free(qu);
}

void TestarrayQueue_get_item_at_idx(
    CuTest * tc
)
{
    void *qu;

    char *item = "testitem", *item2 = "testitem2";

    qu = arrayqueue_new();

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
    void *qu;

    char *item = "testitem", *item2 = "testitem2";

    qu = arrayqueue_new();

    arrayqueue_offer(qu, item);
    arrayqueue_offer(qu, item2);
    arrayqueue_poll(qu);
    CuAssertTrue(tc, item2 == arrayqueue_get_from_idx(qu,0));
    arrayqueue_free(qu);
}

