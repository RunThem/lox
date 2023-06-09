/* libs */

#include <u/fs.h>
#include <u/list.h>
#include <u/map.h>
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
} token_kind_e;

typedef struct {
  token_kind_e type;
  str_t tok;
  int line;
} token_t;

typedef struct {
  str_t code;
  int pos;
  int line;
  list(token_t) toks;
} tokens_t;

map(c_str, token_kind_e) keys = nullptr;

void token_init() {
  map_init(&keys);

  map_push(&keys, "class", T_CLASS);
  map_push(&keys, "else", T_ELSE);
  map_push(&keys, "false", T_FALSE);
  map_push(&keys, "for", T_FOR);
  map_push(&keys, "fn", T_FN);
  map_push(&keys, "if", T_IF);
  map_push(&keys, "nil", T_NIL);
  map_push(&keys, "print", T_PRINT);
  map_push(&keys, "return", T_RETURN);
  map_push(&keys, "super", T_SUPER);
  map_push(&keys, "this", T_THIS);
  map_push(&keys, "true", T_TRUE);
  map_push(&keys, "let", T_LET);
  map_push(&keys, "while", T_WHILE);
}

void lexer_init(tokens_t** self, c_str file) {
  _(self) = u_talloc(sizeof(tokens_t), tokens_t*);

  _(self)->code = fs_read(file);
  _(self)->pos  = 0;
  _(self)->line = 1;
  list_init(&_(self)->toks);
}

void lexer_add(tokens_t** self, token_t tok) {
  tok.line = _(self)->line;

  list_push_b(&_(self)->toks, tok);
}

tokens_t* lexer(c_str file) {
  tokens_t* tokens = nullptr;

  lexer_init(&tokens, file);

#define pop()                (tokens->code->c_str[tokens->pos++])
#define peek()               (tokens->code->c_str[tokens->pos + 1])
#define make(t, str, arg...) lexer_add(&tokens, (token_t){.type = (t), .tok = str_new((str), arg)})

  while (tokens->pos < tokens->code->len) {
    char ch = pop();

    switch (ch) {
      /* clang-format off */
      case '(': { make(T_L_PAREN,   ch); break; }
      case ')': { make(T_R_PAREN,   ch); break; }
      case '{': { make(T_L_BRACE,   ch); break; }
      case '}': { make(T_R_BRACE,   ch); break; }
      case ';': { make(T_SEMICOLON, ch); break; }
      case ',': { make(T_COMMA,     ch); break; }
      case '.': { make(T_DOT,       ch); break; }
      case '+': { make(T_ADD,       ch); break; }
      case '-': { make(T_SUB,       ch); break; }
      case '*': { make(T_MUL,       ch); break; }
      case '/': { make(T_DIV,       ch); break; }
      case '%': { make(T_MOD,       ch); break; }
      /* clang-format on */
      default:
        break;
    }
  }

  make(T_EOF, c('\0'));

#undef pop
#undef peek
#undef make

  return tokens;
}

int main(int argc, const char** argv) {

  inf("hello lox");

  token_init();

  tokens_t* tokens = lexer("lox.x");

  map_for(&keys, it) {
    inf("type(%d), '%s'", it->value, it->key);
  }

  list_for(&tokens->toks, it) {
    inf("type(%d), '%s'", it->val.type, it->val.tok->c_str);
  }

  goto cleanup;
cleanup:
  str_cleanup(&tokens->code);

  list_for(&tokens->toks, it) {
    str_cleanup(&_(it).val.tok);
  }

  list_cleanup(&tokens->toks);

  u_free(tokens);

  return 0;
}
