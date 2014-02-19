#include <stdio.h>
#include <string.h>
#include "read.h"
#include "print.h"

#define DEF_READ_TEST(type_, reader_, printer_) void test_read_##type_(char *s) { test_reader(#type_ , reader_, s, printer_); }


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

DEF_READ_TEST(integer, Scm_read_integer, Scm_print_integer);
DEF_READ_TEST(real, Scm_read_real, Scm_print_real);
DEF_READ_TEST(character, Scm_read_character, Scm_print_character);

int main()
{


  test_read_integer("1");
  test_read_integer("-3");
  test_read_integer("121");
  test_read_integer("-772");

  test_read_real("1.121");
  test_read_real("-65.23");
  test_read_real(".22");
  test_read_real("+.12");
  
  test_read_character("#\\a");
  test_read_character("#\\1");
  test_read_character("#\\F");
  test_read_character("#\\newline");

  return 0;
}

