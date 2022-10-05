#include <check.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include "circular_buffer.h"

void setup(void)
{ }

void teardown(void)
{ }

START_TEST(test_push_three)
{
  struct circ_buf* stringbuf;
  char* a = "abc";
  char* b = "defg";
  char* c = "hi";
  char* output_a;
  char* output_b;
  char* output_c;

  stringbuf = buffer_init(sizeof(char*), 3, false);
  buffer_push(stringbuf, &a);
  buffer_push(stringbuf, &b);
  buffer_push(stringbuf, &c);
  buffer_pop(stringbuf, &output_a);
  buffer_pop(stringbuf, &output_b);
  buffer_pop(stringbuf, &output_c);

  ck_assert_str_eq(a, output_a);
  ck_assert_str_eq(b, output_b);
  ck_assert_str_eq(c, output_c);

  buffer_destroy(stringbuf);
}
END_TEST

START_TEST(test_empty_buf)
{
  struct circ_buf* intbuf;
  int a = 34;
  int b = 11;
  int c = 66;
  int d = 82;
  int e = 1;
  int output_a;
  int output_b;
  int output_c;

  intbuf = buffer_init(sizeof(int), 3, false);
  buffer_push(intbuf, &a);
  buffer_push(intbuf, &b);
  buffer_push(intbuf, &c);
  buffer_push(intbuf, &d);
  buffer_push(intbuf, &e);
  buffer_pop(intbuf, &output_a);
  buffer_pop(intbuf, &output_b);
  buffer_pop(intbuf, &output_c);

  ck_assert_int_eq(c, output_a);
  ck_assert_int_eq(d, output_b);
  ck_assert_int_eq(e, output_c);

  buffer_destroy(intbuf);
}
END_TEST

typedef struct
{
  struct circ_buf* blockbuf;
  int* outputs;
  size_t len;
} pop_struct;

void* pop(void* p)
{
  pop_struct* d = (pop_struct *) p;

  for (size_t i = 0; i < (*d).len; ++i)
  {
    buffer_pop((*d).blockbuf, &(*d).outputs[i]);
  }

  pthread_exit(NULL);
}

START_TEST(test_blocking)
{
  struct circ_buf* blockbuf;
  int inputs[] = {34, 11, 66};
  int outputs[] = {0, 0, 0};
  size_t input_len = sizeof(inputs) / sizeof(int);

  blockbuf = buffer_init(sizeof(int), input_len, false);

  pop_struct d = { .blockbuf = blockbuf, .outputs = &outputs[0], .len = input_len };
  
  pthread_t foo;
  pthread_create(&foo, NULL, pop, &d);
  
  for (size_t i = 0; i < input_len; ++i)
  {
    buffer_push(blockbuf, &inputs[i]);
    sleep(1);
  }

  pthread_join(foo, NULL);

  for (size_t i = 0; i < input_len; ++i)
  {
    ck_assert_int_eq(inputs[i], outputs[i]);
  }

  buffer_destroy(blockbuf);
}
END_TEST

START_TEST(test_nonblocking)
{
  struct circ_buf* nbbuf;
  int a = 2;
  int b = 3;
  int c = 4;
  int output_a;
  int output_b;
  int output_c;
  int ret_a;
  int ret_b;
  int ret_c;
  
  nbbuf = buffer_init(sizeof(int), 3, true);

  buffer_push(nbbuf, &a);
  ret_a = buffer_pop(nbbuf, &output_a);
  ret_b = buffer_pop(nbbuf, &output_b);
  buffer_push(nbbuf, &b);
  buffer_push(nbbuf, &c);
  ret_c = buffer_pop(nbbuf, &output_c);

  ck_assert_int_eq(ret_a, 0);
  ck_assert_int_eq(ret_b, -1);
  ck_assert_int_eq(ret_c, 0);
  ck_assert_int_eq(output_a, a);
  ck_assert_int_eq(output_c, b);

  buffer_destroy(nbbuf);
}
END_TEST

Suite * make_circular_buffer_test_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Circular buffer test suite");

  /* Creation test case */
  tc_core = tcase_create("Test Cases");

  tcase_add_checked_fixture(tc_core, setup, teardown);
  tcase_add_test(tc_core, test_push_three);
  tcase_add_test(tc_core, test_empty_buf);
  tcase_add_test(tc_core, test_blocking);
  tcase_add_test(tc_core, test_nonblocking);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  SRunner *sr;

  sr = srunner_create(make_circular_buffer_test_suite());
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_set_log (sr, "test.log");
  srunner_set_xml (sr, "test.xml");
  srunner_run_all(sr, CK_VERBOSE);

  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}