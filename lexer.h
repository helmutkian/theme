#ifndef _LEXER_H_
#define _LEXER_H_

#include "scm_value.h"

typedef enum {
    TOKEN_ERROR
  , TOKEN_EOF
  , TOKEN_SEMICOLON
  , TOKEN_DOT
  , TOKEN_OPEN_PAREN
  , TOKEN_CLOSE_PAREN
  , TOKEN_LITERAL
} Scm_Token;

typedef struct {
  char *buf;
  unsigned int pos;
  Scm_Value *value;
} Scm_Lexer;

Scm_Token Scm_lex(Scm_Lexer *lexer);

#endif
