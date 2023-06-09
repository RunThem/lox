#include "token.h"

#include <u/u.h>

struct {
  kind_e kind;
  c_str key;
} _keys[100] = {
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
