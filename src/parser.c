#include "parser.h"
#include "token.h"

#undef dis
#define dis(fmt, ...) fprintf(stderr, fmt __VA_OPT__(, ) __VA_ARGS__)

typedef struct {
  token_t cur;
  token_t pre;
  bool has_err; /* 如果有错误, 为真 */
} parser_t;

parser_t parse = {0};

static void err_at(token_t* tok, c_str msg) {
  dis("[line %d] error:", tok->line);

  if (tok->kind == T_EOF) {
    dis(" at end");
  } else if (tok->kind == T_ERROR) {
    /* TODO: nothing */
  } else {
    dis(" at '%s'", tok->tok->c_str);
  }

  dis(": %s\n", msg);
  parse.has_err = true;
}

static void err_at_cur(c_str msg) {
  err_at(&parse.cur, msg);
}

static void err_at_pre(c_str msg) {
  err_at(&parse.pre, msg);
}

void parser(tokens_t* tokens) {
  list_for(&tokens->toks, it) {
    inf("%d", it->val.kind);
  }
}
