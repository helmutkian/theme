#include "read.h"
#include <ctype.h>
#include <string.h>

#define DEF_READER(type_) Scm_ReaderResult Scm_read_##type_(FILE *in, Scm_Value *val)

void ungets(char *s, FILE *f)
{
  int i;
  
  for (i = 0; s[i] != 0; i++) {
    ungetc(s[i], f);
  }
}
  

DEF_READER(integer)
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

DEF_READER(real)
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
    
DEF_READER(character)
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
    

  

  
	
	
    
