#include "lexer.h"

#include <u/fs.h>

#define pop()     (tokens->code->c_str[tokens->pos++])
#define peek()    (tokens->code->c_str[tokens->pos])
#define match(ch) (peek() == c(ch) && (++tokens->pos))
#define make(t)   lexer_add(&tokens, (token_t){.kind = (t)})

static void lexer_init(tokens_t** self, c_str file);
static void lexer_add(tokens_t** self, token_t tok);
static void skip_white_space(tokens_t* tokens);
static void make_string(tokens_t* tokens);
static bool is_eof(tokens_t* tokens);

static void lexer_init(tokens_t** self, c_str file) {
  _(self) = u_talloc(sizeof(tokens_t), tokens_t*);

  _(self)->code = fs_read(file);
  _(self)->pos  = 0;
  _(self)->line = 1;
  list_init(&_(self)->toks);
}

static void lexer_add(tokens_t** self, token_t tok) {
  tok.line = _(self)->line;

  list_push_b(&_(self)->toks, tok);
}

static void skip_white_space(tokens_t* tokens) {
  while (true) {
    switch (peek()) {
      case '#':
        while (peek() != '\n') {
          tokens->pos++;
        }
      case '\n':
        tokens->line++;
      case ' ':
      case '\r':
      case '\t':
        tokens->pos++;
      default:
        return;
    }
  }
  return;
}

static void make_string(tokens_t* tokens) {
  size_t start = tokens->pos;
  while (peek() != '"' && !is_eof(tokens)) {
    tokens->pos++;
  }

  lexer_add(&tokens,
            (token_t){.kind = T_STRING,
                      .tok  = str_new(&tokens->code->c_str[start], tokens->pos - start)});

  tokens->pos++; /* skip '"' */
}

static bool is_eof(tokens_t* tokens) {
  return tokens->pos >= tokens->code->len;
}

tokens_t* lexer(c_str file) {
  tokens_t* tokens = nullptr;

  lexer_init(&tokens, file);

  while (!is_eof(tokens)) {
    skip_white_space(tokens);

    char ch = pop();

    switch (ch) {
      /* clang-format off */
      case '(': { make(T_L_PAREN  ); break; }
      case ')': { make(T_R_PAREN  ); break; }
      case '{': { make(T_L_BRACE  ); break; }
      case '}': { make(T_R_BRACE  ); break; }
      case ';': { make(T_SEMICOLON); break; }
      case ',': { make(T_COMMA    ); break; }
      case '.': { make(T_DOT      ); break; }
      case '+': { make(T_ADD      ); break; }
      case '-': { make(T_SUB      ); break; }
      case '*': { make(T_MUL      ); break; }
      case '/': { make(T_DIV      ); break; }
      case '%': { make(T_MOD      ); break; }
      case '!': { make(match('=') ? T_B_EQUAL : T_BANG);    break; }
      case '=': { make(match('=') ? T_E_EQUAL : T_EQUAL);   break; }
      case '<': { make(match('=') ? T_L_EQUAL : T_LESS);    break; }
      case '>': { make(match('=') ? T_G_EQUAL : T_GREATER); break; }
      case '"': { make_string(tokens); break; }
      /* clang-format on */
      default:
        break;
    }
  }

  make(T_EOF);

  return tokens;
}

#undef pop
#undef peek
#undef match
#undef make
