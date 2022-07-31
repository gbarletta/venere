#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include <iostream>
#include <string>

enum token_type {
    TOKEN_INVALID = -1,
    TOKEN_EOF,
    TOKEN_FUNCTION,
    TOKEN_LET,
    TOKEN_LITERAL,
    TOKEN_NUMBER,
    TOKEN_INTEGER,
    TOKEN_REAL,
    TOKEN_STRING,
    TOKEN_IDENTIFIER,
    TOKEN_EQUAL,
    TOKEN_EQUALEQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_LESS_THAN,
    TOKEN_LESSEQ_THAN,
    TOKEN_GREAT_THAN,
    TOKEN_GREATEQ_THAN,
    TOKEN_SEMICOLON,
    TOKEN_OPENPAREN,
    TOKEN_CLOSEPAREN,
    TOKEN_OPENBRACKET,
    TOKEN_CLOSEBRACKET,
    TOKEN_OPENBRACE,
    TOKEN_CLOSEBRACE,
    TOKEN_QUOTE,
    TOKEN_DOT,
    TOKEN_FOR,
    TOKEN_RETURN,
    TOKEN_WHILE,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NULL,
    TOKEN_PLUS,
    TOKEN_DASH,
    TOKEN_SLASH,
    TOKEN_STAR,
    TOKEN_EXCL,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_INCR,
    TOKEN_DECR,
};

class token {
public:
    token_type type;
    std::string content;

    token();
    token(token_type type);
    token(token_type type, std::string content);

    void print();
};

#endif