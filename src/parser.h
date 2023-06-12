#include "token.h"

typedef struct __ast_t {
  str_t value;
  struct __ast_t* left;
  struct __ast_t* right;
} ast_t;

void parser(tokens_t* tokens);
