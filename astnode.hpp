#ifndef __ASTNODE_HPP__
#define __ASTNODE_HPP__

#include <iostream>
#include <vector>
#include <string>

enum identifier_type {
    TYPE_INVALID = -1,
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_STRING,
    TYPE_ARR_INTEGER,
    TYPE_ARR_REAL,
    TYPE_ARR_STRING,
};

enum ast_type {
    AST_INVALID = -1,
    AST_PROGRAM, 
    AST_BLOCK,
    AST_STATEMENT,
    AST_EXPRESSION,
    AST_SUM,
    AST_SUBTRACT,
    AST_IDENTIFIER,
    AST_FUNCCALL,
    AST_FUNCCALL_ARGS,
    AST_FUNCCALL_CLLE,
    AST_VARDECL, // 10
    AST_FUNCDECL,
    AST_FUNCDECL_ARG,
    AST_FUNCDECL_ARGS,
    AST_FUNCDECL_BODY, 
    AST_RETURN,
    AST_MULTIPLY,
    AST_DIVIDE,
    AST_NUMBER,
    AST_LITERAL,
    AST_TRUE,
    AST_FALSE,
    AST_NULL,
    AST_NEGATE,
    AST_MINUS,
    AST_GREAT_THAN,
    AST_LESS_THAN,
    AST_GREATEQ_THAN,
    AST_LESSEQ_THAN,
    AST_EQUAL,
    AST_NOT_EQUAL,
    AST_POINTER,
    AST_ASSIGNMENT,
    AST_IF,
    AST_CONDITION,
    AST_IF_BODY,
    AST_IF_ELSE,
    AST_WHILE,
    AST_WHILE_BODY,
    AST_FOR,
    AST_FOR_INIT,
    AST_FOR_STEP,
    AST_FOR_BODY,
    AST_AND,
    AST_OR,
    AST_ARRAY_ACCESS,
};

class ast_node_info {
public:
    int number;
    std::string literal;
    std::string identifier;
    identifier_type type;
    
    ast_node_info();
    ast_node_info(std::string str);
    ast_node_info(std::string str, identifier_type type);
    ast_node_info(int number);
};

class ast_node {
public:
    ast_type type;
    ast_node_info info;
    std::vector<ast_node> children;

    ast_node();
    ast_node(ast_type type);
    ast_node(ast_type type, ast_node_info info);

    void add_child(ast_node child);
    void add_children(std::vector<ast_node> children);
    void print(int level);
    void print();
};

#endif