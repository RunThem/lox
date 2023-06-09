/* libs */
#include "lexer.h"
#include "token.h"

tokens_t* tokens = nullptr;

static void cleanup() {
  str_cleanup(&tokens->code);

  list_for(&tokens->toks, it) {
    if (it->val.tok != nullptr)
      str_cleanup(&_(it).val.tok);
  }

  list_cleanup(&tokens->toks);

  u_free(tokens);
}

int main(int argc, const char** argv) {

  /*
   * lexer
   * */
  tokens = lexer("lox.x");

  list_for(&tokens->toks, it) {
    inf("type(%d), '%s'", it->val.kind, keys_rof(&it->val));
  }

  /*
   * cleanup
   * */
  cleanup();

  return 0;
}
