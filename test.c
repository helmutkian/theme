#include <stdio.h>
#include <string.h>
#include "read.h"
#include "print.h"

#define GET_READER(type_) Scm_read_##type_
#define GET_PRINTER(type_) Scm_print_##type_
#define GET_READ_TEST(type_) test_read_##type_

#define DEF_READ_TEST(type_) \
  void GET_READ_TEST(type_)(char *s) \
  { \
    test_reader(#type_ , GET_READER(type_), s, GET_PRINTER(type_)); \
  }

#define RUN_READ_TEST(type_, s) GET_READ_TEST(type_)(s)


void test_reader(char *test_name, Scm_Reader reader, char *expected, Scm_Printer printer)
{
  Scm_Value val;
  FILE *in;

  in = fmemopen(expected, strlen(expected), "r");

  puts(test_name);


  if (reader(in, &val)) {
    printf("\tExpected: %s\n", expected);
    printf("\tSaw: ");
    printer(stdout, &val);
  } else {
    puts("\tRead failed");
  }

  printf("\n");

  fclose(in);
}

DEF_READ_TEST(integer)
DEF_READ_TEST(real)
DEF_READ_TEST(character);

int main()
{


  RUN_READ_TEST(integer, "1");
  RUN_READ_TEST(integer, "-3");
  RUN_READ_TEST(integer, "121");
  RUN_READ_TEST(integer, "-772");

  RUN_READ_TEST(real, "1.121");
  RUN_READ_TEST(real, "-65.23");
  RUN_READ_TEST(real, ".22");
  RUN_READ_TEST(real, "+.12");
  
  RUN_READ_TEST(character, "#\\a");
  RUN_READ_TEST(character, "#\\1");
  RUN_READ_TEST(character, "#\\F");
  RUN_READ_TEST(character, "#\\newline");

  return 0;
}

