#include <stdio.h>
#include <string.h>
#include "read.h"
#include "print.h"

#define GET_READER(type_) read_##type_
#define GET_PRINTER(type_) print_##type_
#define GET_READ_TEST(type_) test_read_##type_

#define DEF_READ_TEST(type_) \
    void GET_READ_TEST(type_)(char *s, int should_pass)\
    {\
      test_reader(#type_ , GET_READER(type_), s, GET_PRINTER(type_), should_pass);\
    }

#define RUN_READ_TEST(type_, s_, pass_) GET_READ_TEST(type_)(s_, pass_)

enum { EXPECT_FAIL, EXPECT_PASS };

// ************************************************************
// Reader test constructor
// ************************************************************

void test_reader(char *test_name, reader reader, char *input, printer printer, int should_pass)
{
  struct value val;
  int result, success;
  char output[256] = {0};
  FILE *in, *out;

  in = fmemopen(input, strlen(input), "r");
  out = fmemopen(output, 256, "w");

  puts(test_name);
  result = reader(in, &val);
  
  if (should_pass && (READ_SUCCESS == result)) {
    printer(out, &val);
    fclose(out);
    if (!strcmp(output, input)) 
      success = 1;
    else 
      success = 0;
  } else if ((!should_pass && (READ_SUCCESS == result)) || should_pass) {
    success = 0;
  } else {
    success = 1;
  }
	     
  if (success) 
    puts("\tPASS");
  else if (!success && (READ_SUCCESS == result)) 
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

