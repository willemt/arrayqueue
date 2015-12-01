
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
    void *qu = aqueue_new(10, 1);
    CuAssertTrue(tc, aqueue_is_empty(qu));
    aqueue_free(qu);
}

void TestarrayQueue_offer_adds_new_item(
    CuTest * tc
    )
{
    char *item = "testitem";
    void *qu = aqueue_new(10, 8);

    aqueue_offer(qu, item);
    CuAssertTrue(tc, 1 == aqueue_count(qu));
    aqueue_free(qu);
}

void TestarrayQueue_cannot_offer_over_capacity(
    CuTest * tc
    )
{
    char *item = "testitem";
    void *qu = aqueue_new(1, 8);

    aqueue_offer(qu, item);
    CuAssertTrue(tc, -1 == aqueue_offer(qu, item));
    CuAssertTrue(tc, 1 == aqueue_count(qu));
    aqueue_free(qu);
}

void TestarrayQueue_peek_gets_head(
    CuTest * tc
    )
{
    char *item1 = "testitem1";
    char *item2 = "testitem2";
    void *qu = aqueue_new(10, 9);

    aqueue_offer(qu, item1);
    aqueue_offer(qu, item2);
    CuAssertTrue(tc, 0 == strncmp(item1, aqueue_peek(qu), 9));
    aqueue_free(qu);
}

void TestarrayQueue_empty_empties_queue(
    CuTest * tc
    )
{
    char *item = "testitem";
    void *qu = aqueue_new(10, 8);

    aqueue_offer(qu, item);
    aqueue_empty(qu);
    CuAssertTrue(tc, 0 == aqueue_count(qu));
    CuAssertTrue(tc, aqueue_is_empty(qu));
    aqueue_free(qu);
}

void TestarrayQueue_cant_poll_with_no_contents(
    CuTest * tc
    )
{
    char *item = "testitem";
    void *qu = aqueue_new(10, 8);
    aqueue_offer(qu, item);
    CuAssertTrue(tc, 0 == strncmp(item, aqueue_peek(qu), 8));
    CuAssertTrue(tc, 0 == aqueue_poll(qu));
    CuAssertTrue(tc, 0 == aqueue_count(qu));
    aqueue_free(qu);
}

void TestarrayQueue_offer_and_poll_item(
    CuTest * tc
    )
{
    char *item = "testitem";
    void *qu = aqueue_new(10, 8);

    aqueue_offer(qu, item);
    CuAssertTrue(tc, 0 == strncmp(item, aqueue_peek(qu), 8));
    CuAssertTrue(tc, 0 == aqueue_poll(qu));
    aqueue_free(qu);
}

void TestarrayQueue_fifo(
    CuTest * tc
    )
{
    char *item = "testitem1", *item2 = "testitem2";
    void *qu = aqueue_new(10, 9);

    aqueue_offer(qu, item);
    aqueue_offer(qu, item2);
    CuAssertTrue(tc, 0 == strncmp(item, aqueue_peek(qu), 9));
    CuAssertTrue(tc, 0 == aqueue_poll(qu));
    CuAssertTrue(tc, 0 == strncmp(item2, aqueue_peek(qu), 9));
    CuAssertTrue(tc, 0 == aqueue_poll(qu));
    aqueue_free(qu);
}

void TestarrayQueue_poll_offer_past_boundary(
    CuTest * tc
    )
{
    char *item1 = "testitem1";
    char *item2 = "testitem2";
    char *item3 = "testitem3";
    void *qu = aqueue_new(2, 9);

    aqueue_offer(qu, item1);
    aqueue_offer(qu, item2);
    CuAssertTrue(tc, 0 == strncmp(item1, aqueue_peek(qu), 9));
    CuAssertTrue(tc, 0 == aqueue_poll(qu));
    CuAssertTrue(tc, 0 == strncmp(item2, aqueue_peek(qu), 9));
    aqueue_offer(qu, item3);
    CuAssertTrue(tc, 0 == strncmp(item2, aqueue_peek(qu), 9));
    CuAssertTrue(tc, 0 == aqueue_poll(qu));
    CuAssertTrue(tc, 0 == strncmp(item3, aqueue_peek(qu), 9));
    CuAssertTrue(tc, 0 == aqueue_poll(qu));
    aqueue_free(qu);
}

void TestarrayQueue_offerensure_ensures_capacity(
    CuTest * tc
    )
{
    char *item = "testitem";
    arrayqueue_t *qu = aqueue_new(1, 8);

    CuAssertTrue(tc, 0 == aqueue_offerensure(&qu, item));
    CuAssertTrue(tc, 1 == aqueue_count(qu));
    CuAssertTrue(tc, 1 == aqueue_size(qu));
    CuAssertTrue(tc, 0 == aqueue_offerensure(&qu, item));
    CuAssertTrue(tc, 2 == aqueue_count(qu));
    CuAssertTrue(tc, 2 == aqueue_size(qu));
    aqueue_free(qu);
}

void TestarrayQueue_iterator_iterates(
    CuTest * tc
    )
{
    char *item = "testitem";
    char *item2 = "TESTITEM";
    arrayqueue_t *qu = aqueue_new(1, 8);

    CuAssertTrue(tc, 0 == aqueue_offerensure(&qu, item));
    CuAssertTrue(tc, 0 == aqueue_offerensure(&qu, item2));

    arrayqueue_iter_t iter;
    aqueue_iter(qu, &iter);
    CuAssertTrue(tc, 1 == aqueue_iter_has_next(qu, &iter));
    CuAssertTrue(tc, 0 == strncmp(item, aqueue_iter_next(qu, &iter), 8));
    CuAssertTrue(tc, 1 == aqueue_iter_has_next(qu, &iter));
    CuAssertTrue(tc, 0 == strncmp(item2, aqueue_iter_next(qu, &iter), 8));
    CuAssertTrue(tc, 0 == aqueue_iter_has_next(qu, &iter));
    aqueue_free(qu);
}
