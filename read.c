#include "read.h"
#include <ctype.h>
#include <string.h>


void ungets(char *s, FILE *f)
{
  int i;
  
  for (i = 0; s[i] != 0; i++) {
    ungetc(s[i], f);
  }
}

int isdelim(char c)
{
  
  return isspace(c) || (EOF == c) || ('\'' == c) || (';' == c);
}

/* (+|-|/0)[0-9]+ where /0 is a null token */
int read_fixnum(FILE *in, struct value *val)
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
    } else if (isdelim(c)) {
      ungetc(c, in);
      break;
    } else {
      ungets(buf, in);
      return READ_INVALID_CHAR;
    }
    i++;
  }

  if (buf[0] != 0) {
    val->type = FIXNUM;
    val->fixnum = atoi(buf) * (neg ? -1 : 1);
    return READ_SUCCESS;
  } else {
    return READ_FAIL;
  }
}

/* (+|-|\0)[0-9]*.[0-9]+ */
int read_flonum(FILE *in, struct value *val)
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

  while (i < BUF_LEN) {
    c = fgetc(in);
    if ('.' == c)
      decimal_seen = 1;
    else if (!isdigit(c)) 
      break;
    buf[i] = c;
    i++;
  }

  if (decimal_seen) {
    val->flonum = atof(buf) * (neg ? -1 : 1);
    val->type = FLONUM;
    return READ_SUCCESS;  
  } else {
    ungets(buf, in);
    return READ_FAIL;
  }
}
    
int read_character(FILE *in, struct value *val)
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

  if (special_str[0]) {
    special_len = strlen(special_str);
    for (i = 1; i < special_len; i++) {
      buf[i-1] = c = fgetc(in);
      if (tolower(c) != special_str[i]) {
	ungets(buf, in);
	return READ_INVALID_CHAR;
      }
    }
    val->character = special_char;
  } else {
    val->character = c;
  }

  return READ_SUCCESS;
}

int read_string(FILE *in, struct value *val)
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
      return READ_MISMATCH_DELIM;
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
	return READ_INVALID_CHAR;
      }
    } else {
      val->string.arr[i] = c;
    }
  }

  val->type = STRING;
  val->string.len = i;
  return READ_SUCCESS;
}

// ([0-9]|(~!@#$%^&*-+=\/)|[a-z])*[a-z]+([0-9]|(~!@#$%^&*-+=\/)|[a-z])
int read_symbol(FILE *in, struct value *val)
{
  char c;
  const char valid_chars[] = "~!@#$%^&*-+=/\\";
  unsigned int i, non_numerical_seen = 0;

  for (i = 0; i < MAX_SYMBOL_SIZE; i++) {
    c = fgetc(in);
    if (isdelim(c)) {
      break;
    } else if (!(isalnum(c) || strchr(valid_chars, c))) {
      val->symbol[i+1] = 0;
      ungets(val->symbol, in);
      return READ_INVALID_CHAR;
    } else if (!isdigit(c) && !non_numerical_seen) {
      non_numerical_seen = 1;
    }
    val->symbol[i] = c;
  }
  val->symbol[i] = 0;
   
  if (!val->symbol[0] || !non_numerical_seen) {
    ungets(val->symbol, in);
    return READ_FAIL;
  } else {
    val->type = SYMBOL;
    return READ_SUCCESS;
  }
}
      
// ************************************************************
// ************************************************************


