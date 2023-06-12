#include "parser.h"
#include "token.h"

#include <stdio.h>
#include <stdlib.h>

#undef dis
#define dis(fmt, ...) fprintf(stderr, fmt __VA_OPT__(, ) __VA_ARGS__)

typedef struct {
  token_t cur;
  token_t pre;
  bool has_err; /* 如果有错误, 为真 */
} parser_t;

parser_t parse = {0};

list_iter(token_t) tok = nullptr;

/* static function */
static void pop();
static ast_t* new_node(ast_t* left, ast_t* right, token_t tok);
static void err_at(token_t* tok, c_str msg);
static void err_at_cur(c_str msg);
static void err_at_pre(c_str msg);
static void consume(kind_e kind, c_str msg);
static bool is_kind(kind_e kind);
static ast_t* expr();
static ast_t* mul();
static ast_t* primary();

static void pop() {
  parse.cur = parse.pre;

  if (tok != nullptr) {
    parse.pre = tok->val;
    tok       = tok->next;
  }
}

static ast_t* new_node(ast_t* left, ast_t* right, token_t tok) {
  ast_t* node = u_talloc(sizeof(ast_t), ast_t*);
  node->left  = left;
  node->right = right;
  node->tok   = tok;

  return node;
}

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

ast_t* parser(tokens_t* tokens) {
  tok = list_get_iter(&tokens->toks);

  pop();
  pop();

  return expr();
}

static void consume(kind_e kind, c_str msg) {
  if (parse.cur.kind == kind) {
    pop();
    return;
  }

  err_at_cur(msg);
}

static bool is_kind(kind_e kind) {
  if (parse.cur.kind == kind) {
    return true;
  }

  return false;
}

static ast_t* expr() {
  ast_t* node = mul();

  while (true) {
    if (is_kind(T_ADD) || is_kind(T_SUB)) {
      auto t = parse.cur;
      pop();
      node = new_node(node, mul(), t);
    } else {
      break;
    }
  }

  return node;
}

static ast_t* mul() {
  ast_t* node = primary();

  while (true) {
    if (is_kind(T_MUL) || is_kind(T_DIV)) {
      auto t = parse.cur;
      pop();
      node = new_node(node, primary(), t);
    } else {
      break;
    }
  }

  return node;
}

static ast_t* primary() {
  ast_t* node = nullptr;

  if (is_kind(T_R_PAREN)) {
    node = expr();

    consume(T_R_PAREN, "Expect ')' after expression."); /* skip ')' */
  } else {
    node = new_node(nullptr, nullptr, parse.cur);
    pop();
  }

  return node;
}
