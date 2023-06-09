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

struct {
  kind_e kind;
  c_str key;
} _keys[] = {
    {T_L_PAREN,   "("     },
    {T_R_PAREN,   ")"     },
    {T_L_BRACE,   "{"     },
    {T_R_BRACE,   "}"     },
    {T_COMMA,     ","     },
    {T_SEMICOLON, ";"     },
    {T_DOT,       "."     },
    {T_ADD,       "+"     },
    {T_SUB,       "-"     },
    {T_MUL,       "/"     },
    {T_DIV,       "*"     },
    {T_MOD,       "%"     },
    {T_BANG,      "!"     },
    {T_B_EQUAL,   "!="    },
    {T_EQUAL,     "="     },
    {T_E_EQUAL,   "=="    },
    {T_GREATER,   ">"     },
    {T_G_EQUAL,   ">="    },
    {T_LESS,      "<"     },
    {T_L_EQUAL,   "<="    },
    {T_AND,       "&&"    },
    {T_OR,        "||"    },
    {T_CLASS,     "class" },
    {T_ELSE,      "else"  },
    {T_FALSE,     "false" },
    {T_FOR,       "for"   },
    {T_FN,        "fn"    },
    {T_IF,        "if"    },
    {T_NIL,       "nil"   },
    {T_PRINT,     "print" },
    {T_RETURN,    "return"},
    {T_SUPER,     "super" },
    {T_THIS,      "this"  },
    {T_TRUE,      "true"  },
    {T_LET,       "let"   },
    {T_WHILE,     "while" },
    {T_EOF,       "EOF"   },
};

kind_e keys_of(token_t* tok) {
  for (size_t i = 0; i < arrlen(_keys); i++) {
    if (!strcmp(tok->tok->c_str, _keys[i].key)) {
      return _keys[i].kind;
    }
  }

  return T_EOF;
}

c_str keys_rof(token_t* tok) {
  if (tok->kind == T_STRING || tok->kind == T_NUMBER || tok->kind == T_IDENT)
    return tok->tok->c_str;

  for (size_t i = 0; i < arrlen(_keys); i++) {
    if (_keys[i].kind == tok->kind) {
      return _keys[i].key;
    }
  }

  return "";
}

void skip_white_space(tokens_t* tokens) {
  while (true) {
    switch (tokens->code->c_str[tokens->pos]) {
      case '#':
        while (tokens->code->c_str[tokens->pos] != '\n') {
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

void make_string(tokens_t* tokens) {
  size_t start = tokens->pos;
  while (tokens->code->c_str[tokens->pos] != '"' && tokens->pos < tokens->code->len) {
    tokens->pos++;
  }

  lexer_add(&tokens,
            (token_t){.kind = T_STRING,
                      .tok  = str_new(&tokens->code->c_str[start], tokens->pos - start)});

  tokens->pos++; /* skip '"' */
}

tokens_t* lexer(c_str file) {
  tokens_t* tokens = nullptr;

  lexer_init(&tokens, file);

#define pop()     (tokens->code->c_str[tokens->pos++])
#define peek()    (tokens->code->c_str[tokens->pos])
#define match(ch) (peek() == c(ch) && (++tokens->pos))
#define make(t)   lexer_add(&tokens, (token_t){.kind = (t)})

  while (tokens->pos < tokens->code->len) {
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

#undef pop
#undef peek
#undef make

  return tokens;
}

int main(int argc, const char** argv) {

  inf("hello lox");

  tokens_t* tokens = lexer("lox.x");

  list_for(&tokens->toks, it) {
    inf("type(%d), '%s'", it->val.kind, keys_rof(&it->val));
  }

  goto cleanup;
cleanup:
  str_cleanup(&tokens->code);

  list_for(&tokens->toks, it) {
    if (it->val.tok != nullptr)
      str_cleanup(&_(it).val.tok);
  }

  list_cleanup(&tokens->toks);

  u_free(tokens);

  return 0;
}
