#include "scheme_parser.h"
#include "read_stream.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Maximum number of digits in numbers (fixnums/flonums)
#define NUM_BUF_SIZE 100
#define CHAR_BUF_SIZE 15
#define MIN_CHAR_BUF_SIZE 3
#define STR_ARR_SIZE 256


// Test to see if character is a delimiter
static int is_delim(char c);


int 
is_delim(char c)
{
  return (c == EOF) || isspace(c);
}



// ************************************************************
// Parsers
// ************************************************************

scheme_parse_ret 
parse_number(FILE *stream, scheme_data **data)
{
  char c;
  char buf[NUM_BUF_SIZE];
  read_stream *rstream;
  scheme_parse_ret success = PARSE_SUCCESS;

  rstream = read_stream_make(stream);
    
  c = read_stream_get_char(rstream, NULL);
  if (isdigit(c)) goto digit_loop;
  else if ('.' == c) goto mantissa_loop;
  else goto reject;

 digit_loop:
  c = read_stream_get_char(rstream, NULL);
  if (isdigit(c)) goto digit_loop;
  else if ('.' == c) goto mantissa_loop;
  else if (is_delim(c)) goto accept_fixnum;
  else goto reject;

 mantissa_loop:
  c = read_stream_get_char(rstream, NULL);
  if (isdigit(c)) goto mantissa_loop;
  else if (is_delim(c)) goto accept_flonum;
  else goto reject;

 reject:
  read_stream_unwind(rstream);
  success = PARSE_FAIL;
  goto done;

 accept_fixnum:
  *data = scheme_data_alloc(SCHEME_FIXNUM);
  (*data)->i = atoi(read_stream_get_buf(rstream));
  goto done;

 accept_flonum:
  *data = scheme_data_alloc(SCHEME_FLONUM);
  (*data)->f = atof(read_stream_get_buf(rstream));
  goto done;

 done:
  read_stream_free(rstream);
  return success;
 
}


scheme_parse_ret
parse_char(FILE *stream, scheme_data **data)
{
  char *buf, c;
  read_stream *rstream;

  rstream = read_stream_make(stream);
  c = read_stream_get_char(rstream, NULL);
  if (c != '#') goto reject;

  c = read_stream_get_char(rstream, NULL);
  if (c != '\\') goto reject;

  do { 
    c = read_stream_get_char(rstream, " \t\n");
  } while (!is_delim(c));

  buf = read_stream_get_buf(rstream);
  if (strlen(buf) == MIN_CHAR_BUF_SIZE) {
    c = buf[read_stream_get_pos(rstream)];
    goto accept;
  } else if (strcasecmp(buf, "#\\newline") == 0) {
    c = '\n';
    goto accept;
  } else if (strcasecmp(buf, "#\\space") == 0) {
    c = ' ';
    goto accept;
  } else if (strcasecmp(buf, "#\\tab") == 0) {
    c = '\t';
    goto accept;
  } else {
    goto reject;
  }

 reject:
  read_stream_unwind(rstream);
  read_stream_free(rstream);
  return PARSE_FAIL;
 accept:
  *data = scheme_data_alloc(SCHEME_CHAR);
  (*data)->c = c;
  read_stream_free(rstream);
  return PARSE_SUCCESS;
}

/*
scheme_parse_ret
parse_string(FILE *stream, scheme_data **data)
{
  char c;
  char *buf;
  int num_elems;
  int max_size = STR_ARR_SIZE;

  buf = malloc(sizeof(char) * max_size);
  
  c = _stream_read_char(stream, buf, INIT);
  if ('"' == c) {
  
}
  
void
_resize_arr(char **arr, int *size)
{
  char *new_arr;
  int new_size = *size * 2;

  size *= 2;
  new_arr = malloc(sizeof(char) * new_size);
  memcpy(*new_arr, arr, size);
  *arr = new_arr;
  *size = new_size;
}
  
*/
