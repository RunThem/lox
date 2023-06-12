/* libs */
#include "lexer.h"
#include "parser.h"
#include "token.h"

tokens_t* tokens = nullptr;
ast_t* asts      = nullptr;

static void cleanup() {
  str_cleanup(&tokens->code);

  list_for(&tokens->toks, it) {
    if (it->val.tok != nullptr)
      str_cleanup(&_(it).val.tok);
  }

  list_cleanup(&tokens->toks);

  u_free(tokens);
}

static void ast_dis(ast_t* ast) {
  if (ast == nullptr) {
    return;
  }

  fprintf(stderr, "(");

  if (ast->left != nullptr) {
    ast_dis(ast->left);
  }

  fprintf(stderr, "%s", keys_rof(&ast->tok));

  if (ast->right != nullptr) {
    ast_dis(ast->right);
  }

  fprintf(stderr, ")");
}

int main(int argc, const char** argv) {

  /*
   * lexer
   * */
  tokens = lexer("lox.x");

  if (argc == 2 && !strcmp(argv[1], "-l")) {
    list_for(&tokens->toks, it) {
      inf("%d: type(%d), '%s'", it->val.line, it->val.kind, keys_rof(&it->val));
    }
  }

  asts = parser(tokens);

  ast_dis(asts);
  fprintf(stderr, "\n");

  /*
   * cleanup
   * */
  cleanup();

  return 0;
}
