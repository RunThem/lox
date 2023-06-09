#include "parser.h"

void parser(tokens_t* tokens) {
  list_for(&tokens->toks, it) {
    inf("%d", it->val.kind);
  }
}
