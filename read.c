#include "read.h"
#include <ctype.h>
#include <string.h>


void ungets(char *s, FILE *f)
{
  int len = strlen(s);

  fseek(f, -len, SEEK_CUR);

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
  unsigned int digit_seen = 0, i = 0;

  c = fgetc(in);
  
  if (isdigit(c)) {
    digit_seen = 1;
  } else if ((c != '+') && (c != '-')) {
    ungetc(c, in);
    return READ_FAIL;
  } 

  buf[i++] = c;
    
  while (i < BUF_LEN) {
    c = fgetc(in);
    buf[i++] = c;
    if (isdelim(c)) {
      break;
    } else if (isdigit(c)) { 
      digit_seen = 1;
    } else {
      ungets(buf, in);
      return READ_INVALID_CHAR;
    }
  }

  if (digit_seen) {
    val->type = FIXNUM;
    val->fixnum = atoi(buf);
    ungetc(c, in); // Return delimiter to stream.
    return READ_SUCCESS;
  } else {
    ungets(buf, in);
    return READ_FAIL;
  }
}

/* (+|-|\0)[0-9]*.[0-9]+ */
int read_flonum(FILE *in, struct value *val)
{
  char c;
  char buf[BUF_LEN] = {0};
  unsigned int i = 0, digit_seen = 0, decimal_seen = 0;
    
  c = fgetc(in);

  if ('.' == c) {
    decimal_seen = 1;
    buf[i++] = '0';
  } else if (isdigit(c)) {
    digit_seen = 1;
  } else if ((c != '+') && (c != '-')) {
    ungetc(c, in);
    return READ_FAIL;
  }

  buf[i++] = c;

  while (i < BUF_LEN) {
    c = fgetc(in);
    buf[i++] = c;
    if ('.' == c) {
      decimal_seen = 1;
      if (!digit_seen)
	buf[i++] = '0';
      else 
	digit_seen = 0;
    }
    else if (!isdigit(c)) { 
      break;
    }
    digit_seen = 1;
  }

  if (decimal_seen && digit_seen) {
    val->flonum = (double)atof(buf);
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

  val->type = CHARACTER;
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
    } else if (!non_numerical_seen && !isdigit(c)) {
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
  
/*    
int read_pair(FILE *in, struct value *val)
{
  char c;
  struct value *pair;
  Stack stack = NULL;
  int dotted = 0;

  c = fgetc(in);
  if ('(' == c) {
    val->type = PAIR;
    pair = val;
  }
      
  for (;;) {
    c = fgetc(in);
    if ('(' == c) {
      pair->car = scm_alloc();
      pair->car->type = PAIR;
      stack_push(stack, pair);
      pair = stack_top(stack)->car;
    } else if ('.' == c) { 
      dotted = 1;
    } else if (')' == c) {
      if (!dotted) 
	pair->cdr = NULL;
      pair = stack_pop(stack);
    } else if ((dotted && !(read(in, pair->cdr) != READ_SUCCESS))
	       read(in, pair->car) != READ_SUCCESS) {
      return READ_FAIL;
    }

    pair->cdr = scm_alloc();
    pair->cdr->type = PAIR;
    pair = pair->cdr;
  }

  
}
*/

int read(FILE *in, struct value *val)
{
  reader readers[] = {read_fixnum, read_flonum, read_character, read_string, read_symbol, /* read_pair, */ NULL};
  int i, result;

  for (i = 0; readers[i] != NULL; i++) {
    result = readers[i](in, val);
    if (READ_SUCCESS == result) 
      return READ_SUCCESS;
    else if (READ_MISMATCH_DELIM == result)
      return READ_MISMATCH_DELIM;
  }

  return READ_FAIL;
}
  


