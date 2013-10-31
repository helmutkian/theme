#include "scheme_parser.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Maximum number of digits in numbers (fixnums/flonums)
#define NUM_BUF_SIZE 100
#define CHAR_BUF_SIZE 15
#define MIN_CHAR_BUF_SIZE 3

// Values for controlling reading from stream via _stream_read_char
enum { CURR_CHAR       // Return current character in buffer
     , NEXT_CHAR       // Read next character, ignoring delims
     , NEXT_CHAR_DELIM // Read nexr character, including delims
     , INIT            // Initialize reader 
};          

// Test to see if character is a delimiter
static int is_delim(char c);

// Reads a char from stream into the given buffer
static char _stream_read_char(FILE *stream, char *buf, const int control);

// Returns all chars from buffer back onto stream
static void _stream_unwind(FILE *stream, char *buf);



// ************************************************************
// Stream utility functions
// ************************************************************

char
_stream_read_char(FILE *stream, char *buf, const int control)
{
  static int pos = -1;
  static char c;

  if (control == CURR_CHAR) {
    return (pos == -1) ? '\0' : buf[pos];
  } else if (control == INIT) {
    pos = -1;
  }

  c = fgetc(stream);

  if (!is_delim(c) || (control == NEXT_CHAR_DELIM)) {
    buf[++pos] = c;
    buf[pos + 1] = '\0';
  }

  return c;
}



static void
_stream_unwind(FILE *stream, char *buf)
{
  int i;

  for (i = strlen(buf) - 1; i >= 0; i--) {
    ungetc(buf[i], stream);
  }
}


// ************************************************************
// Other utility functions
// ************************************************************

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

  // FIXNUM: [0-9]+
  // FLONUM: [0-9]*.[0-9]+
  // NUMBER: FIXNUM | FLONUM

  c = _stream_read_char(stream, buf, INIT);
  if (isdigit(c)) goto digit_loop;
  else if ('.' == c) goto mantissa_loop;
  else goto reject;

 digit_loop:
  c = _stream_read_char(stream, buf, NEXT_CHAR);
  if (isdigit(c)) goto digit_loop;
  else if ('.' == c) goto mantissa_loop;
  else if (is_delim(c)) goto accept_fixnum;
  else goto reject;

 mantissa_loop:
  c = _stream_read_char(stream, buf, NEXT_CHAR);
  if (isdigit(c)) goto mantissa_loop;
  else if (is_delim(c)) goto accept_flonum;
  else goto reject;

 reject:
  _stream_unwind(stream, buf);
  return PARSE_FAIL;

 accept_fixnum:
  *data = scheme_data_alloc(SCHEME_FIXNUM);
  (*data)->i = atoi(buf);
  return PARSE_SUCCESS;

 accept_flonum:
  *data = scheme_data_alloc(SCHEME_FLONUM);
  (*data)->f = atof(buf);
  return PARSE_SUCCESS; 
 
}


scheme_parse_ret
parse_char(FILE *stream, scheme_data **data)
{
  char c;
  char buf[CHAR_BUF_SIZE];
 
  c = _stream_read_char(stream, buf, INIT);
  if (c != '#') goto reject;

  c = _stream_read_char(stream, buf, NEXT_CHAR);
  if (c != '\\') goto reject;

  do { 
    c = _stream_read_char(stream, buf, NEXT_CHAR);
  } while (!is_delim(c));

  if (strlen(buf) == MIN_CHAR_BUF_SIZE) {
    c = _stream_read_char(stream, buf, CURR_CHAR);
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
  _stream_unwind(stream, buf);
  return PARSE_FAIL;
 accept:
  *data = scheme_data_alloc(SCHEME_CHAR);
  (*data)->c = c;
  return PARSE_SUCCESS;
}



  
