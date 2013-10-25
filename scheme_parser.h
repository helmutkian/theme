#ifndef SCHEME_PARSER_H
#define SCHEME_PARSER_H

#include "scheme_data.h"

enum {
  PARSE_FAIL
  , PARSE_SUCCESS
};

// Parses a number from a C-string
int parse_number(char *, scheme_data_t **);

// Parses a character from a C-string
int parse_char(char *, scheme_data_t **);

// Parses a Scheme string from a C-string
int parse_string(char *, scheme_data_t **);

#endif
