#include "token.h"

typedef struct __ast_t {
  token_t tok;
  struct __ast_t* left;
  struct __ast_t* right;
} ast_t;

ast_t* parser(tokens_t* tokens);
