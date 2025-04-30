#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "queue.h"

static void test_queue_init(void **state) {
    queue_t* que = queue_create();
    assert_non_null(que);
    queue_destroy(que);
}

static void test_queue_enqueue_when_q_empty(void **state) {
    queue_t* que = queue_create();
    request_t* req = malloc(sizeof(request_t));
    req->id = 1;
    req->data = 100;

    queue_enqueue(que, req);

    assert_int_equal(que->front->req->id, 1);
    assert_int_equal(que->rear->req->data, 100);
    assert_true(que->front == que->rear);

    free(req);
    queue_destroy(que);
}

static void test_queue_dequeue(void **state) {
    queue_t* que = queue_create();
    request_t* req1 = malloc(sizeof(request_t));
    req1->id = 1;
    req1->data = 100;

    request_t* req2 = malloc(sizeof(request_t));
    req2->id = 2;
    req2->data = 200;

    queue_enqueue(que, req1);
    queue_enqueue(que, req2);

    request_t* deq = queue_dequeue(que);

    assert_int_equal(deq->id, 1);
    assert_int_equal(que->front->req->id, 2);

    free(deq);
    free(req2);
    queue_destroy(que);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_queue_init),
        cmocka_unit_test(test_queue_enqueue_when_q_empty),
        cmocka_unit_test(test_queue_dequeue),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
