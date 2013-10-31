#ifndef SCHEME_PARSER_H
#define SCHEME_PARSER_H

#include "scheme_data.h"
#include <stdio.h>

typedef enum {
  PARSE_FAIL
  , PARSE_SUCCESS
} scheme_parse_ret;

// Parses a number from a C-string
scheme_parse_ret parse_number(FILE *, scheme_data **);

// Parses a character from a C-string
scheme_parse_ret parse_char(FILE *, scheme_data **);

// Parses a Scheme string from a C-string
//int parse_string(FILE *, scheme_data **);

#endif
