
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
    void *qu = aqueuenp_new(10, 1);
    CuAssertTrue(tc, aqueuenp_is_empty(qu));
    aqueuenp_free(qu);
}

void TestarrayQueue_offer_adds_new_item(
    CuTest * tc
    )
{
    char *item = "testitem";
    void *qu = aqueuenp_new(10, 8);

    aqueuenp_offer(qu, item);
    CuAssertTrue(tc, 1 == aqueuenp_count(qu));
    aqueuenp_free(qu);
}

void TestarrayQueue_cannot_offer_over_capacity(
    CuTest * tc
    )
{
    char *item = "testitem";
    void *qu = aqueuenp_new(1, 8);

    aqueuenp_offer(qu, item);
    CuAssertTrue(tc, -1 == aqueuenp_offer(qu, item));
    CuAssertTrue(tc, 1 == aqueuenp_count(qu));
    aqueuenp_free(qu);
}

void TestarrayQueue_peek_gets_head(
    CuTest * tc
    )
{
    char *item1 = "testitem1";
    char *item2 = "testitem2";
    void *qu = aqueuenp_new(10, 9);

    aqueuenp_offer(qu, item1);
    aqueuenp_offer(qu, item2);
    CuAssertTrue(tc, 0 == strncmp(item1, aqueuenp_peek(qu), 9));
    aqueuenp_free(qu);
}

void TestarrayQueue_empty_empties_queue(
    CuTest * tc
    )
{
    char *item = "testitem";
    void *qu = aqueuenp_new(10, 8);

    aqueuenp_offer(qu, item);
    aqueuenp_empty(qu);
    CuAssertTrue(tc, 0 == aqueuenp_count(qu));
    CuAssertTrue(tc, aqueuenp_is_empty(qu));
    aqueuenp_free(qu);
}

void TestarrayQueue_cant_poll_with_no_contents(
    CuTest * tc
    )
{
    char *item = "testitem";
    void *qu = aqueuenp_new(10, 8);
    aqueuenp_offer(qu, item);
    CuAssertTrue(tc, 0 == strncmp(item, aqueuenp_peek(qu), 8));
    CuAssertTrue(tc, 0 == aqueuenp_poll(qu));
    CuAssertTrue(tc, 0 == aqueuenp_count(qu));
    aqueuenp_free(qu);
}

void TestarrayQueue_offer_and_poll_item(
    CuTest * tc
    )
{
    char *item = "testitem";
    void *qu = aqueuenp_new(10, 8);

    aqueuenp_offer(qu, item);
    CuAssertTrue(tc, 0 == strncmp(item, aqueuenp_peek(qu), 8));
    CuAssertTrue(tc, 0 == aqueuenp_poll(qu));
    aqueuenp_free(qu);
}

void TestarrayQueue_fifo(
    CuTest * tc
    )
{
    char *item = "testitem1", *item2 = "testitem2";
    void *qu = aqueuenp_new(10, 9);

    aqueuenp_offer(qu, item);
    aqueuenp_offer(qu, item2);
    CuAssertTrue(tc, 0 == strncmp(item, aqueuenp_peek(qu), 9));
    CuAssertTrue(tc, 0 == aqueuenp_poll(qu));
    CuAssertTrue(tc, 0 == strncmp(item2, aqueuenp_peek(qu), 9));
    CuAssertTrue(tc, 0 == aqueuenp_poll(qu));
    aqueuenp_free(qu);
}

void TestarrayQueue_poll_offer_past_boundary(
    CuTest * tc
    )
{
    char *item1 = "testitem1";
    char *item2 = "testitem2";
    char *item3 = "testitem3";
    void *qu = aqueuenp_new(2, 9);

    aqueuenp_offer(qu, item1);
    aqueuenp_offer(qu, item2);
    CuAssertTrue(tc, 0 == strncmp(item1, aqueuenp_peek(qu), 9));
    CuAssertTrue(tc, 0 == aqueuenp_poll(qu));
    CuAssertTrue(tc, 0 == strncmp(item2, aqueuenp_peek(qu), 9));
    aqueuenp_offer(qu, item3);
    CuAssertTrue(tc, 0 == strncmp(item2, aqueuenp_peek(qu), 9));
    CuAssertTrue(tc, 0 == aqueuenp_poll(qu));
    CuAssertTrue(tc, 0 == strncmp(item3, aqueuenp_peek(qu), 9));
    CuAssertTrue(tc, 0 == aqueuenp_poll(qu));
    aqueuenp_free(qu);
}

void TestarrayQueue_offerensure_ensures_capacity(
    CuTest * tc
    )
{
    char *item = "testitem";
    arrayqueuenp_t *qu = aqueuenp_new(1, 8);

    CuAssertTrue(tc, 0 == aqueuenp_offerensure(&qu, item));
    CuAssertTrue(tc, 1 == aqueuenp_count(qu));
    CuAssertTrue(tc, 1 == aqueuenp_size(qu));
    CuAssertTrue(tc, 0 == aqueuenp_offerensure(&qu, item));
    CuAssertTrue(tc, 2 == aqueuenp_count(qu));
    CuAssertTrue(tc, 2 == aqueuenp_size(qu));
    aqueuenp_free(qu);
}
