#include <stdio.h>
#include <string.h>
#include "read.h"
#include "print.h"

#define GET_READER(type_) read_##type_
#define GET_PRINTER(type_) print_##type_
#define GET_READ_TEST(type_) test_read_##type_

#define DEF_READ_TEST(type_) \
  void GET_READ_TEST(type_)(char *s) \
  { \
    test_reader(#type_ , GET_READER(type_), s, GET_PRINTER(type_)); \
  }

#define RUN_READ_TEST(type_, s) GET_READ_TEST(type_)(s)

// ************************************************************
// Reader test constructor
// ************************************************************

void test_reader(char *test_name, reader reader, char *expected, printer printer)
{
  struct value val;
  FILE *in;

  in = fmemopen(expected, strlen(expected), "r");

  puts(test_name);


  if (!reader(in, &val)) {
    printf("\tExpected: %s\n", expected);
    printf("\tSaw: ");
    printer(stdout, &val);
  } else {
    puts("\tRead failed");
  }

  printf("\n");

  fclose(in);
}

// ************************************************************
// Reader test
// ************************************************************

DEF_READ_TEST(fixnum);
DEF_READ_TEST(flonum);
DEF_READ_TEST(character);
DEF_READ_TEST(string);

// ************************************************************
// Run Tests
// ************************************************************

int main()
{

  RUN_READ_TEST(fixnum, "1");
  RUN_READ_TEST(fixnum, "-3");
  RUN_READ_TEST(fixnum, "121");
  RUN_READ_TEST(fixnum, "-772");

  RUN_READ_TEST(flonum, "1.121");
  RUN_READ_TEST(flonum, "-65.23");
  RUN_READ_TEST(flonum, ".22");
  RUN_READ_TEST(flonum, "+.12");
  
  RUN_READ_TEST(character, "#\\a");
  RUN_READ_TEST(character, "#\\1");
  RUN_READ_TEST(character, "#\\F");
  RUN_READ_TEST(character, "#\\newline");

  RUN_READ_TEST(string, "\"hello world\"");
  RUN_READ_TEST(string, "\" \\\"quoted\\\" \"");

  return 0;
}

