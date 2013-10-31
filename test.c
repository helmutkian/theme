#include "scheme_data.h"
#include "scheme_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef scheme_parse_ret (*parser_f)(FILE *, scheme_data **);

void test_parser(char *input, parser_f parser )
{
  scheme_data *data;
  FILE * stream;

  stream = fmemopen(input, strlen(input), "r");
  
  if (!parser(stream, &data)) {
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
  test_parser("#\\newline", parse_char);
  test_parser("#\\space", parse_char);
  test_parser("#\\tab", parse_char);
  test_parser("#\\a", parse_char); 

  return 0;
}


