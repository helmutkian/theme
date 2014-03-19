#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read.h"
#include "print.h"

#define GET_READER(type_) read_##type_
#define GET_PRINTER(type_) print_##type_
#define GET_READ_TEST(type_) test_read_##type_

#define DEF_READ_TEST(type_) \
  void GET_READ_TEST(type_)(char *s, char *e, int should_pass)	\
    {\
      test_reader(#type_ , GET_READER(type_), s, e, GET_PRINTER(type_), should_pass); \
    }

#define RUN_READ_TEST(type_, s_, e_, pass_) GET_READ_TEST(type_)(s_, e_, pass_)

enum { EXPECT_FAIL, EXPECT_PASS };

// ************************************************************
// Reader test constructor
// ************************************************************

void test_reader(char *test_name, reader reader, char *input, char *expected, printer printer, int should_pass)
{
  struct value val;
  int buf_size, pass = 0;
  char output[256];
  FILE *in, *out;

  in = fmemopen(input, strlen(input), "r");
  out = fmemopen(output, sizeof(output), "w");

  puts(test_name);

  if (reader(in, &val) == READ_SUCCESS) {
    printer(out, &val);
    fclose(out);
    if (!strcmp(output, expected))
      pass = 1;
  }
  
  if ((pass && should_pass) || (!pass && !should_pass))
    puts("\tPASS");
  else if (!pass && should_pass) 
    printf("\tFAIL: Expected: %s\n\t      Saw: %s", input, output);
  else 
    printf("\tFAIL: Failed to reject %s", input);
  
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

  RUN_READ_TEST(fixnum, "1", "1", EXPECT_PASS);
  RUN_READ_TEST(fixnum, "-3", "-3", EXPECT_PASS);
  RUN_READ_TEST(fixnum, "121", "121", EXPECT_PASS);
  RUN_READ_TEST(fixnum, "-772", "-772", EXPECT_PASS);
  RUN_READ_TEST(fixnum, "12a", NULL, EXPECT_FAIL); 
  RUN_READ_TEST(fixnum, "12.0", NULL, EXPECT_FAIL);
  RUN_READ_TEST(fixnum, "12'a", "12", EXPECT_PASS);

  RUN_READ_TEST(flonum, "1.121", "1.121", EXPECT_PASS);
  RUN_READ_TEST(flonum, "-65.23", "-65.23", EXPECT_PASS);
  RUN_READ_TEST(flonum, ".22", "0.22", EXPECT_PASS);
  RUN_READ_TEST(flonum, "+.12", "0.12", EXPECT_PASS);
  
  RUN_READ_TEST(character, "#\\a", "a", EXPECT_PASS);
  RUN_READ_TEST(character, "#\\1", "1", EXPECT_PASS);
  RUN_READ_TEST(character, "#\\F", "F", EXPECT_PASS);
  RUN_READ_TEST(character, "#\\newline", "\n", EXPECT_PASS);
  RUN_READ_TEST(character, "#\\newfoo", NULL, EXPECT_FAIL);
  RUN_READ_TEST(character, "#n", NULL, EXPECT_FAIL);

  RUN_READ_TEST(string, "\"hello world\"", "hello world", EXPECT_PASS);
  RUN_READ_TEST(string, "\" \\\"quoted\\\" \"", " \"quoted\" ", EXPECT_PASS);
  RUN_READ_TEST(string, "\"quotes unclosed", NULL, EXPECT_FAIL);

  return 0;
}

