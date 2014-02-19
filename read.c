#include "read.h"
#include <ctype.h>
#include <string.h>

// ************************************************************
// ************************************************************
// DEF_READER : Template for reader function headers
// ************************************************************
#define DEF_READER(type_, strm_, val_) Scm_ReaderResult GET_READER(type_)(FILE *strm_, Scm_Value *val_)
// ************************************************************
// DEF_READER(foo, input_stream, scheme_data) =>
// Scm_ReaderResult Scm_read_foo(FILE *input_stream, Scm_Value scheme_data)
// ************************************************************
// ************************************************************

void ungets(char *s, FILE *f)
{
  int i;
  
  for (i = 0; s[i] != 0; i++) {
    ungetc(s[i], f);
  }
}
  

DEF_READER(integer, in, val)
{
  char c;
  char buf[BUF_LEN] = {0};
  unsigned int i = 0, neg = 0;

  c = fgetc(in);

  if ('-' == c) {
    neg = 1;
  } else if (isdigit(c)) {
    buf[i++] = c;
  } else if ((EOF == c) || (c != '+')) {
    ungetc(c, in);
    return READ_FAIL;
  }
    
  while ((EOF != (c = fgetc(in))) && (i < BUF_LEN)) {
    if (isdigit(c)) { 
      buf[i] = c;
    } else if ('.' == c) {
      ungets(buf, in);
      return READ_FAIL;
    } else {
      break;
    }
    i++;
  }

  if (buf[0] != 0) {
    val->type = INTEGER;
    val->integer = atoi(buf) * (neg ? -1 : 1);
    return READ_SUCCESS;
  } else {
    return READ_FAIL;
  }
}

DEF_READER(real, in, val)
{
  char c;
  char buf[BUF_LEN] = {0};
  unsigned int i = 0, neg = 0, decimal_seen = 0;
    
  c = fgetc(in);
 
  if ('-' == c) {
    neg = 1;
  } else if (isdigit(c)) {
    buf[i++] = c;
  } else if ('.' == c) {
    decimal_seen = 1;
    buf[i++] = '0';
    buf[i++] = '.';
  } else if ((EOF == c) || (c != '+')) {
    ungetc(c, in);
    return READ_FAIL;
  }

  while ((EOF != (c = fgetc(in))) && (i < BUF_LEN)) {
    if (isdigit(c)) {
      buf[i] = c;
    } else if ('.' == c) {
      buf[i] = c;
      decimal_seen = 1;
    } else {
      break;
    }
    i++;
  }

  if (decimal_seen) {
    val->real = atof(buf) * (neg ? -1 : 1);
    val->type = REAL;
    return READ_SUCCESS;
  } else {
    ungets(buf, in);
    return READ_FAIL;
  }
}
    
DEF_READER(character, in, val)
{
  char c, special_char;
  char buf[BUF_LEN] = {0};
  char special_str[BUF_LEN] = {0};
  unsigned int special_len, i;

  if ((c = fgetc(in)) != '#') {
    ungetc(c, in);
    return READ_FAIL;
  }

  if ((c = fgetc(in)) != '\\') {
    ungetc(c, in);
    return READ_FAIL;
  }

  c = fgetc(in);

  if (tolower(c) == 'n') {
    special_char = '\n';
    strcpy(special_str, "newline");
  } else if (tolower(c) == 't') {
    special_char = '\t';
    strcpy(special_str, "tab");
  } else if (tolower(c) == 's') {
    special_char = ' ';
    strcpy(special_str, "space");
  }

  if (special_str[0] != 0) {
    special_len = strlen(special_str);
    for (i = 1; i < special_len; i++) {
      buf[i-1] = c = fgetc(in);
      if (tolower(c) != special_str[i]) {
	ungets(buf, in);
	break;
      }
    }
    val->character = special_char;
  } else {
    val->character = c;
  }

  val->type = CHARACTER;
  return READ_SUCCESS;
}

DEF_READER(string, in, val)
{
  char c;
  unsigned int i;

  if ((c = fgetc(in)) != '\"') {
    ungetc(c, in);
    return READ_FAIL;
  }

  for (i = 0; (c = fgetc(in)) != '\"'; i++) {
    if (EOF == c) {
      val->string.arr[i] = 0;
      ungets(val->string.arr, in);
      return READ_FAIL;
    } else if ('\\' == c) {
      c = fgetc(in);
      if ('n' == tolower(c)) {
	val->string.arr[i] = '\n';
      } else if ('t' == tolower(c)) {
	val->string.arr[i] = '\t';
      } else if ('\"' == c) {
	val->string.arr[i] = '\"';
      } else {
	val->string.arr[i] = 0;
	ungets(val->string.arr, in);
	return READ_FAIL;
      }
    } else {
      val->string.arr[i] = c;
    }
  }

  val->type = STRING;
  val->string.len = i;
  return READ_SUCCESS;
}
      
// ************************************************************
// ************************************************************

DEF_READER(atom, in, val)
{
  unsigned int i;
  Scm_Reader atom_readers[4] = { 
      GET_READER(integer)
    , GET_READER(real)
    , GET_READER(character)
    , GET_READER(string)
  };
  

  for (i = 0; i < 4; i++) {
    if (atom_readers[i](in, val)) {
      return READ_SUCCESS;
    }
  }

  return READ_FAIL;
}
