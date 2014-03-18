#include <stdio.h>
#include <string.h>
#include "read.h"
#include "print.h"

#define GET_READER(type_) read_##type_
#define GET_PRINTER(type_) print_##type_
#define GET_READ_TEST(type_) test_read_##type_

#define DEF_READ_TEST(type_) \
    void GET_READ_TEST(type_)(char *s, int success)\
    {\
      test_reader(#type_ , GET_READER(type_), s, GET_PRINTER(type_), success);\
    }

#define RUN_READ_TEST(type_, s_, pass_) GET_READ_TEST(type_)(s_, pass_)

enum { EXPECT_FAIL, EXPECT_PASS };

// ************************************************************
// Reader test constructor
// ************************************************************

void test_reader(char *test_name, reader reader, char *input, printer printer, int success)
{
  struct value val;
  FILE *in;

  in = fmemopen(input, strlen(input), "r");

  puts(test_name);


  if ((READ_SUCCESS == reader(in, &val)) && success) {
    printf("\tInput: %s\n", input);
    printf("\tSaw: ");
    printer(stdout, &val);
  } else if (!success) {
    printf("\tSuccessfully rejected: %s\n", input);
  } else {
    puts("\tRead test failed");
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

  RUN_READ_TEST(fixnum, "1", EXPECT_PASS);
  RUN_READ_TEST(fixnum, "-3", EXPECT_PASS);
  RUN_READ_TEST(fixnum, "121", EXPECT_PASS);
  RUN_READ_TEST(fixnum, "-772", EXPECT_PASS);
  RUN_READ_TEST(fixnum, "12a", EXPECT_FAIL); 

  RUN_READ_TEST(flonum, "1.121", EXPECT_PASS);
  RUN_READ_TEST(flonum, "-65.23", EXPECT_PASS);
  RUN_READ_TEST(flonum, ".22", EXPECT_PASS);
  RUN_READ_TEST(flonum, "+.12", EXPECT_PASS);
  
  RUN_READ_TEST(character, "#\\a", EXPECT_PASS);
  RUN_READ_TEST(character, "#\\1", EXPECT_PASS);
  RUN_READ_TEST(character, "#\\F", EXPECT_PASS);
  RUN_READ_TEST(character, "#\\newline", EXPECT_PASS);

  RUN_READ_TEST(string, "\"hello world\"", EXPECT_PASS);
  RUN_READ_TEST(string, "\" \\\"quoted\\\" \"", EXPECT_PASS);

  return 0;
}

