#include "scheme_data.h"
#include "scheme_parser.h"
#include <stdio.h>
#include <stdlib.h>

typedef int (*parser_f)(char *, scheme_data_t **);

void test_parser(char *input, parser_f parser )
{
  scheme_data_t *data;
  
  if (!parser(input, &data)) {
    puts("ERROR");
  } else {
    scheme_print_data(data);
    puts("");
    scheme_free_data(data);
  }

}

  
int main() 
{

  test_parser("12", parse_number);
  test_parser("1.2", parse_number);
  test_parser("#\\&", parse_char);
  test_parser("\"no escape chars\"", parse_string);
  test_parser("\"\\\"escape\\\"\"", parse_string);

  return 0;
}


