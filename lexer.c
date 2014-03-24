#include "lexer.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define cur(lexer_) (lexer_)->buf[(lexer_)->pos]

static int _is_delim(char c);
static Scm_Token _find_start_pos(Scm_Lexer *lexer);
static Scm_Token _lex_string(Scm_Lexer *lexer);
static Scm_Token _lex_character(Scm_Lexer *lexer);

int _is_delim(char c)
{
  char delims[] = "();";

  return isspace(c) || (EOF == c) || strchr(delims, c);
}
 
// Advances the position of the lexer in its buffer beyond any
// leading whitespace or comments.
//
// Returns TOKEN_ERROR if comments are misformatted, !TOKEN_ERROR
// otherwise.
Scm_Token _find_start_pos(Scm_Lexer *lexer)
{
  while (cur(lexer)) {
    if (';' == cur(lexer)) { // Endline comments
      do {
	lexer->pos++;
	// Endline comment must end in newline
	if (!cur(lexer)) 
	  return TOKEN_ERROR;
      } while ('\n' != cur(lexer));
    } else if ('#' == cur(lexer)) { // Block comments
      lexer->pos++;
      if ('|' == cur(lexer)) {
	// Look for comment block end: "|#"
	for (;;) {
	  lexer->pos++;
	  if ('|' == cur(lexer)) {
	    lexer->pos++;
	    if ('#' == cur(lexer)) 
	      break;
	    else 
	      lexer->pos--;
	  } else if (!cur(lexer)) { // EOF before block end => ERROR
	    return TOKEN_ERROR;
	  }
	}
      }
    } else if (isspace(cur(lexer))) { // Whitespace
      lexer->pos++;
    } else {
      return !TOKEN_ERROR;
    }
  }
}

Scm_Token _lex_string(Scm_Lexer *lexer)
{
  int i;
  lexer->lexeme = Scm_alloc(TYPE_STRING);

  for (i = 0; cur(lexer) != '"'; i++) {
    lexer->pos++;
    if ('\\' == cur(lexer)) { // Handle escape chars
      lexer->pos++;
      if ('n' == tolower(curr(lexer))) 
	lexer->lexeme->string.arr[i] = '\n';
      else if ('t' == tolower(curr(lexer)))
	lexer->lexeme->string.arr[i] = '\t';
      else if ('\\' == curr(lexer)) 
	lexer->lexeme->string.arr[i] = '\\';
      else 
	goto err;
    } else if (!curr(lexer)) { // No close quote
      goto err;
    } else {
      lexer->lexeme->string.arr[i] = curr(lexer);
    }
  }
	
  return TOKEN_LITERAL;

 err:
  Scm_free(lexer->lexeme);
  return TOKEN_ERROR;
}

Scm_Token _lex_character(Scm_Lexer *lexer)
{
  int i = 1; // Position offset to ensure a delimiter after char literal
  char c, named_char;
  char char_name[15] = {0};

  lexer->lexeme = Scm_alloc(TYPE_CHARACTER);

  switch (tolower(curr(lexer))) {
  case 'n':
    strcpy(char_name, "newline");
    named_char = '\n';
    break;
  case 't':
    strcpy(char_name, "tab");
    named_char = '\t';
    break;
  case 's':
    strcpy(char_name, "space");
    named_char = ' ';
    break;
  default: 
    break;
  }

  if (!char_name[0]) {
    lexer->lexeme->character = curr(lexer);
  } else { 
    for (i = 0; char_name[i]; i++) {
      c = lexer->buf[lexer->pos + i];
      if (!c || (tolower(c) != char_name[i])) 
	goto err;
    }
    lexer->lexeme->character = named_char;
  }

  if (_is_delim(lexer->buf[lexer->pos + i]))
    return TOKEN_LITERAL;

 err:
  Scm_free(lexer->lexeme);
  return TOKEN_ERROR;
}

    

Scm_Token Scm_lex(Scm_Lexer *lexer)
{
  if (_find_start_pos(lexer) == TOKEN_ERROR)
    return TOKEN_ERROR;

  if (EOF == cur(lexer)) 
    return TOKEN_EOF;

  switch (lexer->buf[lexer->pos++]) {
  case '.': return TOKEN_DOT;
  case '(': return TOKEN_OPEN_PAREN;
  case ')': return TOKEN_CLOSE_PAREN;
  case '"': return _lex_string(lexer);
  case '#':
    if ('\\' == cur(lexer)) 
      return _lex_character(lexer);
    else 
      return _lex_boolean(lexer);
  default: 
    lexer->pos--;
    break;
  }

}
      
 
    
