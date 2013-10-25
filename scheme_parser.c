#include "scheme_parser.h"
#include <string.h>
#include <stdlib.h>

int parse_number(char *input, scheme_data_t **data)
{
  enum {
    START
    , FLOAT
    , ACCEPT_FIXNUM
    , ACCEPT_FLOAT
    , REJECT
    , DONE
  };

  int state = START;
  int looped, i = 0;
  char c;
  char *buf;

  while (state != DONE) {
    c = input[i++];
    switch (state) {
    case START:
      if (isdigit(c)) {
	looped = 1;
      } else if ('.' == c) {
	state = FLOAT;
	looped = 0;
      } else if (looped) {
	state = ACCEPT_FIXNUM;
      } else {
	state = REJECT;
      }
      break;
    case FLOAT:
      if (isdigit(c)) {
	looped = 1;
      } else if (looped) {
	state = ACCEPT_FLOAT;
      } else {
	state = REJECT;
      }
      break;
    case ACCEPT_FIXNUM:
      buf = calloc(i, sizeof(char));
      memcpy(buf, input, i);
      *data = malloc(sizeof(scheme_data_t));
      (*data)->i = atoi(buf);
      (*data)->type = SCHEME_FIXNUM;
      state = DONE;
      break;
    case ACCEPT_FLOAT:
      buf = calloc(i, sizeof(char));
      memcpy(buf, input, i);
      *data = malloc(sizeof(scheme_data_t));
      (*data)->f = atof(buf);
      (*data)->type = SCHEME_FLOAT;
      state = DONE;
      break;
    case REJECT:
      return PARSE_FAIL;
      break;
    default:
      break;
    }
  }

  free(buf);
  return PARSE_SUCCESS;
}

int parse_char(char *input, scheme_data_t **data)
{
  enum {
    START
    , CHAR_READER
    , CHAR_VAL
    , NEWLINE
    , SPACE
    , TAB
    , ACCEPT_CHAR
    , ACCEPT_NEWLINE
    , ACCEPT_SPACE
    , ACCEPT_TAB
    , REJECT
    , DONE
  };

  int state = START;
  int looped = 0;
  int i = 0;
  int j = 0;

  char newline[] = "newline";
  char space[] = "space";
  char tab[] = "tab";
  char c;

  while (state != DONE) {
    c = input[i++];
    switch (state) {
    case START:
      if ('#' == c) {
	state = CHAR_READER;
      } else {
	state = REJECT;
      }
      break;
    case CHAR_READER:
      if ('\\' == c) {
	state = CHAR_VAL;
      } else {
	state = REJECT;
      }
      break;
    case CHAR_VAL:
      if ('n' == tolower(c)) {
	state = NEWLINE;
      } else if ('s' == tolower(c)) {
	state = SPACE;
      } else if ('t' == tolower(c)) {
	state = TAB;
      } else if (!looped) {
	state = ACCEPT_CHAR;
	looped = 1;
      } else {
	state = REJECT;
      }
      break;
    case NEWLINE:
      if (c == newline[++j]) {
	looped++;
      } else if (!looped) {
	state = ACCEPT_CHAR;
      } else if (strlen(newline) == looped) {
	state = ACCEPT_NEWLINE;
      } else {
	state = REJECT;
      }
      break;
    case SPACE:
      if (c == space[++j]) {
	looped++;
      } else if (!looped) {
	state = ACCEPT_CHAR;
      } else if (strlen(space) == looped) {
	state = ACCEPT_SPACE;
      } else {
	state = REJECT;
      }
      break;
    case TAB:
      if (c == tab[++j]) {
	looped++;
      } else if (!looped) {
	state = ACCEPT_CHAR;
      } else if (strlen(tab) == looped) {
	state = ACCEPT_TAB;
      } else {
	state = REJECT;
      }
      break;
    case ACCEPT_CHAR:
      state = DONE;
      c = input[2];
      break;
    case ACCEPT_NEWLINE:
      state = DONE;
      c = '\n';
      break;
    case ACCEPT_SPACE:
      state = DONE;
      c = ' ';
      break;
    case ACCEPT_TAB:
      state = DONE;
      c = '\t';
      break;
    case REJECT:
      return PARSE_FAIL;
    default:
      break;
    }
  }
  
  *data = (scheme_data_t *)malloc(sizeof(scheme_data_t));
  (*data)->c = c;
  (*data)->type = SCHEME_CHAR;
  return PARSE_SUCCESS;
}

int parse_string(char *input, scheme_data_t **data)
{
  enum {
    START
    , STRING
    , ESCAPE_CHAR
    , REJECT
    , ACCEPT
    , DONE
  };

  const int MIN_ITERS = 3; // Minimum # of iterations of FSM 
  int state = START;
  int i = 0;
  int len;
  char c;

  while (state != DONE) {
    c = input[i++];
    switch (state) {
    case START:
      if ('"' == c) {
	state = STRING;
      } else {
	state = REJECT;
      }
      break;
    case STRING:
      if ('\\' == c) {
	state = ESCAPE_CHAR;
      } else if ('"' == c) {
	state = ACCEPT;
      }
      break;
    case ESCAPE_CHAR:
      state = STRING;
      break;
    case ACCEPT:
      len = i - MIN_ITERS;
      *data = (scheme_data_t *)malloc(sizeof(scheme_data_t));
      (*data)->s.str = (char *)malloc(len * sizeof(char));
      (*data)->s.length = len;
      memcpy((*data)->s.str, input + 1, len);
      state = DONE;
      break;
    case REJECT:
      return PARSE_FAIL;
      break;
    default:
      break;
    }
  }

  (*data)->type = SCHEME_STRING;
  return PARSE_SUCCESS;
}
	
