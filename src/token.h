#pragma once

#include <u/list.h>
#include <u/str.h>
#include <u/u.h>

typedef enum {
  /* 单字符词法 */
  T_L_PAREN,   /* ( */
  T_R_PAREN,   /* ) */
  T_L_BRACE,   /* { */
  T_R_BRACE,   /* } */
  T_COMMA,     /* , */
  T_SEMICOLON, /* ; */
  T_DOT,       /* . */
  T_ADD,       /* + */
  T_SUB,       /* - */
  T_MUL,       /* / */
  T_DIV,       /* * */
  T_MOD,       /* % */

  /* 一或两字符词法 */
  T_BANG,    /* !    */
  T_B_EQUAL, /* !=   */
  T_EQUAL,   /* =    */
  T_E_EQUAL, /* ==   */
  T_GREATER, /* >    */
  T_G_EQUAL, /* >=   */
  T_LESS,    /* <    */
  T_L_EQUAL, /* <=   */
  T_AND,     /* &&   */
  T_OR,      /* ||   */

  /* 字面量 */
  T_IDENT,  /* {ident}  */
  T_STRING, /* {string} */
  T_NUMBER, /* {number} */

  /* 关键字 */
  T_CLASS,  /* class  */
  T_ELSE,   /* else   */
  T_FALSE,  /* false  */
  T_FOR,    /* for    */
  T_FN,     /* fn     */
  T_IF,     /* if     */
  T_NIL,    /* nil    */
  T_PRINT,  /* print  */
  T_RETURN, /* return */
  T_SUPER,  /* super  */
  T_THIS,   /* this   */
  T_TRUE,   /* true   */
  T_LET,    /* let    */
  T_WHILE,  /* while  */

  T_ERROR,
  T_EOF,

  T_MAX,
} kind_e;

typedef struct {
  kind_e kind;
  str_t tok;
  int line;
} token_t;

typedef struct {
  str_t code;
  int pos;
  int line;
  list(token_t) toks;
} tokens_t;

extern kind_e keys_of(token_t* tok);
extern c_str keys_rof(token_t* tok);
extern int precedence(kind_e kind);
