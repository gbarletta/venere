#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <iostream>
#include <vector>
#include "token.hpp"
#include "astnode.hpp"

class parse_result {
public:
    ast_node node;
    std::string error;

    parse_result(std::string error);
    parse_result(ast_node node);

    ast_node unwrap();
};

class parser {
public:
    bool eof;
    std::size_t current_token;
    std::vector<token> token_list;

    parser();

    identifier_type parse_type();
    parse_result parse_primary();
    parse_result parse_call();
    parse_result parse_unary();
    parse_result parse_factor();
    parse_result parse_term();
    parse_result parse_comparison();
    parse_result parse_equality();
    parse_result parse_and();
    parse_result parse_or();
    parse_result parse_assignment();
    parse_result parse_expression();
    parse_result parse_exprstmt();
    parse_result parse_block();
    parse_result parse_if();
    parse_result parse_while();
    parse_result parse_for();
    parse_result parse_return();
    parse_result parse_statement();
    parse_result parse_vardecl();

    ast_node parse();
    bool consume(std::size_t c);
    token peek(std::size_t c);
};

#endif