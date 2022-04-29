#include <iostream>
#include <vector>

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
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NULL,
    TOKEN_PLUS,
    TOKEN_DASH,
    TOKEN_SLASH,
    TOKEN_STAR,
    TOKEN_EXCL,
};

class token {
public:
    token_type type;
    std::string content;

    token() {
        this->type = TOKEN_INVALID;
    }

    token(token_type type) {
        this->type = type;
    }

    token(token_type type, std::string content) {
        this->type = type;
        this->content = content;
    }

    void print() {
        std::cout << content << "\n";
    }
};

enum ast_type {
    AST_INVALID = -1,
    AST_BLOCK,
    AST_STATEMENT,
    AST_EXPRESSION,
    AST_SUM,
    AST_SUBTRACT,
    AST_VARIABLE,
    AST_FUNCCALL,
    AST_VARDECL,
    AST_FUNCDECL,
    AST_FUNCDECL_ARG,
    AST_FUNCDECL_ARGS,
    AST_FUNCDECL_BODY, // 10
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
};

enum identifier_type {
    TYPE_INVALID = -1,
    TYPE_INTEGER,
    TYPE_REAL,
    TYPE_STRING,
    TYPE_ARR_INTEGER,
    TYPE_ARR_REAL,
    TYPE_ARR_STRING,
};

class ast_node_info {
public:
    int number;
    std::string literal;
    std::string identifier;
    identifier_type type;
    

    ast_node_info() {
        this->identifier = "";
    }
    
    ast_node_info(std::string str) {
        this->identifier = str;
        this->literal = str;
    }

    ast_node_info(std::string str, identifier_type type) {
        this->identifier = str;
        this->type = type;
    }

    ast_node_info(int number) {
        this->number = number;
    }
};

class ast_node {
public:
    ast_type type;
    ast_node_info info;
    std::vector<ast_node> children;

    ast_node() {
        this->info = ast_node_info("");
        this->type = AST_BLOCK;
    }
    
    ast_node(ast_type type) {
        this->info = ast_node_info("");
        this->type = type;
    }

    ast_node(ast_type type, ast_node_info info) {
        this->type = type;
        this->info = info;
    }

    void add_child(ast_node child) {
        this->children.push_back(child);
    }

    void add_children(std::vector<ast_node> children) {
        for (std::size_t i = 0; i < children.size(); i++) {
            add_child(children[i]);
        }
    }

    void print(int level) {
        for (int i = 0; i < level; i++) std::cout << "  ";
        std::cout << type << " ";

        if (info.identifier != "") {
            std::cout << info.identifier;
        }

        if (type == AST_NUMBER) {
            std::cout << info.number;
        }

        if (children.size()) {
            std::cout << " my children:\n";
            ++level;
            for (std::size_t i = 0; i < children.size(); i++) {
                children[i].print(level);
            }
        } else {
            std::cout << "\n";
        }
    }

    void print() {
        print(0);
    }
};

class parse_result {
public:
    ast_node node;
    std::string error;

    parse_result(std::string error) {
        node = ast_node(AST_INVALID);
        this->error = error;
    }

    parse_result(ast_node node) {
        this->node = node;
    }

    ast_node unwrap() {
        if (!error.empty()) {
            std::cout << error << "\n";
            std::exit(1);
        }

        return node;
    }
};

class parser {
public:
    bool eof;
    std::size_t current_token;
    std::vector<token> token_list;

    parser() {
        eof = false;
        current_token = 0;
        token_list.push_back(token(TOKEN_LET, "let"));
        token_list.push_back(token(TOKEN_IDENTIFIER, "test"));
        token_list.push_back(token(TOKEN_EQUAL, "="));
        token_list.push_back(token(TOKEN_IDENTIFIER, "arg"));
        token_list.push_back(token(TOKEN_PLUS, "+"));
        token_list.push_back(token(TOKEN_NUMBER, "4"));
        token_list.push_back(token(TOKEN_SEMICOLON, ";"));
    }

    parse_result parse_primary() {
        std::cout << "parse_primary\n";
        token t0 = peek(0);

        switch (t0.type) {
        case TOKEN_IDENTIFIER: {
            std::string identifier = t0.content;
            consume(1);

            if ((t0 = peek(0)).type == TOKEN_OPENPAREN) {
                consume(1);

                ast_node funccall = ast_node(AST_FUNCCALL, identifier);

                while ((t0 = peek(0)).type != TOKEN_INVALID) {
                    ast_node argument = parse_expression().unwrap();
                    funccall.add_child(argument);

                    if ((t0 = peek(0)).type == TOKEN_COMMA) {
                        consume(1);
                        continue;
                    } else if (t0.type == TOKEN_CLOSEPAREN) {
                        consume(1);
                        return parse_result(funccall);
                    } else {
                        return parse_result("',' or ')' expected for function call");
                    }
                }
            } else {
                return parse_result(ast_node(AST_VARIABLE, ast_node_info(identifier)));
            }   
        }
        case TOKEN_NUMBER:
            consume(1);
            return parse_result(ast_node(AST_NUMBER, ast_node_info(std::stoi(t0.content))));
        case TOKEN_LITERAL:
            consume(1);
            return parse_result(ast_node(AST_LITERAL, ast_node_info(t0.content)));
        case TOKEN_TRUE:
            consume(1);
            return parse_result(ast_node(AST_TRUE));
        case TOKEN_FALSE:
            consume(1);
            return parse_result(ast_node(AST_FALSE));
        case TOKEN_NULL:
            consume(1);
            return parse_result(ast_node(AST_NULL));
        case TOKEN_OPENPAREN:
            consume(1);
            ast_node expression = parse_expression().unwrap();
            
            if ((t0 = peek(0)).type == TOKEN_CLOSEPAREN) {
                consume(1);
                return parse_result(expression);
            } else {
                return parse_result("')' expected");
            }
        }
    }

    parse_result parse_unary() {
        std::cout << "parse_unary\n";
        token t0 = peek(0);

        if (t0.type == TOKEN_EXCL) {
            consume(1);
            ast_node node = ast_node(AST_NEGATE);
            node.add_child(parse_unary().unwrap());
            return parse_result(node);
        }

        if (t0.type == TOKEN_DASH) {
            consume(1);
            ast_node node = ast_node(AST_MINUS);
            node.add_child(parse_unary().unwrap());
            return parse_result(node);
        } 

        if (t0.type == TOKEN_STAR) {
            consume(1);
            ast_node node = ast_node(AST_POINTER);
            node.add_child(parse_unary().unwrap());
            return parse_result(node);
        } 

        return parse_result(parse_primary().unwrap());
    }

    parse_result parse_factor() {
        std::cout << "factor\n";
        token t0;
        ast_node unary = parse_unary().unwrap();

        while ((t0 = peek(0)).type == TOKEN_STAR || t0.type == TOKEN_SLASH) {
            consume(1);
            ast_node node = (t0.type == TOKEN_STAR) ? ast_node(AST_MULTIPLY) : ast_node(AST_DIVIDE);
            node.add_child(unary);
            node.add_child(parse_unary().unwrap());
            unary = node;
        }

        return parse_result(unary);
    }

    parse_result parse_term() {
        std::cout << "parse_term\n";
        token t0;
        ast_node factor = parse_factor().unwrap();

        while ((t0 = peek(0)).type == TOKEN_PLUS || t0.type == TOKEN_DASH) {
            consume(1);
            ast_node node = (t0.type == TOKEN_PLUS) ? ast_node(AST_SUM) : ast_node(AST_SUBTRACT);
            node.add_child(factor);
            node.add_child(parse_factor().unwrap());
            factor = node;
        }

        return parse_result(factor);
    }

    parse_result parse_comparison() {
        std::cout << "parse_comparison\n";
        token t0;
        ast_node node;
        ast_node term = parse_term().unwrap();

        while ((t0 = peek(0)).type != TOKEN_INVALID) {
            switch (t0.type) {
            case TOKEN_GREAT_THAN:     
                node = ast_node(AST_GREAT_THAN);
                break;
            case TOKEN_GREATEQ_THAN:
                node = ast_node(AST_GREATEQ_THAN);
                break; 
            case TOKEN_LESS_THAN:
                node = ast_node(AST_LESS_THAN);
                break;
            case TOKEN_LESSEQ_THAN:
                node = ast_node(AST_LESSEQ_THAN);
                break;
            default:
                return parse_result(term);
            }

            consume(1);
            node.add_child(term);
            node.add_child(parse_term().unwrap());
            term = node;
        }

        return parse_result(term);
    }

    parse_result parse_equality() {
        std::cout << "parse_equality\n";
        token t0;
        ast_node node;
        ast_node comparison = parse_comparison().unwrap();

        while ((t0 = peek(0)).type == TOKEN_NOT_EQUAL || t0.type == TOKEN_EQUAL) {
            consume(1);
            node = (t0.type == TOKEN_NOT_EQUAL) ? ast_node(AST_NOT_EQUAL) : ast_node(AST_EQUAL);
            node.add_child(comparison);
            node.add_child(parse_comparison().unwrap());
            comparison = node;
        }

        return parse_result(comparison);
    }

    parse_result parse_assignment() {
        std::cout << "parse_assignment\n";
        token t0 = peek(0);
        token t1 = peek(1);
        ast_node node;

        if (t0.type == TOKEN_IDENTIFIER && t1.type == TOKEN_EQUAL) {
            consume(2);
            node = ast_node(AST_ASSIGNMENT, ast_node_info(t0.content));
            node.add_child(parse_assignment().unwrap());
            return parse_result(node);       
        } 

        return parse_result(parse_equality().unwrap());
    }

    parse_result parse_expression() {
        std::cout << "parse_expression()\n";
        return parse_result(parse_assignment().unwrap());
    }

    parse_result parse_exprstmt() {
        std::cout << "parse_exprstmt()\n";
        ast_node expr = parse_expression().unwrap();
        token t0 = peek(0);

        if (t0.type != TOKEN_SEMICOLON) {
            return parse_result("';' expected after expression-statement");
        }

        consume(1);
        return parse_result(expr);
    }

    parse_result parse_statement() {
        std::cout << "parse_statement()\n";
        return parse_result(parse_exprstmt().unwrap());
    }

    parse_result parse_block() {
        return parse_result(ast_node(AST_INVALID));
    }

    identifier_type parse_type() {
        token t0 = peek(0);
        token t1 = peek(1);
        token t2 = peek(2);

        identifier_type type = TYPE_INVALID;

        if (t0.type != TOKEN_INTEGER && t0.type != TOKEN_REAL && t0.type != TOKEN_STRING) {
            return TYPE_INVALID;
        }

        consume(1);

        if (t1.type == TOKEN_OPENBRACKET && t2.type == TOKEN_CLOSEBRACKET) {
            consume(2);
            switch (t0.type) {
            case TOKEN_INTEGER:
                type = TYPE_ARR_INTEGER;
            case TOKEN_REAL:
                type = TYPE_ARR_REAL;
            case TOKEN_STRING:
                type = TYPE_ARR_STRING;
            }  
        } else {
            switch (t0.type) {
            case TOKEN_INTEGER:
                type = TYPE_INTEGER;
            case TOKEN_REAL:
                type = TYPE_REAL;
            case TOKEN_STRING:
                type = TYPE_STRING;
            }
        }

        return type;
    }

    parse_result parse_vardecl() {
        token t0 = peek(0);
        token t1 = peek(1);
        token t2 = peek(2);
        ast_node node;

        if (t0.type == TOKEN_LET) {
            if (t1.type != TOKEN_IDENTIFIER) {
                return parse_result("identifier expected for variable declaration");
            }

            if (t2.type != TOKEN_COLON) {
                return parse_result("':' expected for variable declaration");
            }
            
            consume(3);
        
            identifier_type var_type = parse_type();
            if (var_type == TYPE_INVALID) {
                return parse_result("type expected for variable declaration");
            }

            node = ast_node(AST_VARDECL, ast_node_info(t1.content, var_type));

            if ((t0 = peek(0)).type == TOKEN_EQUAL) {
                consume(1);
                node.add_child(parse_expression().unwrap());
            }

            if ((t0 = peek(0)).type != TOKEN_SEMICOLON) {
                return parse_result("';' expected in variable declaration");
            }

            consume(1);
            return parse_result(node);
        } else {
            std::cout << "we " << t0.content << "\n";
            std::cout << "we " << t1.content << "\n";
            std::cout << "we " << t2.content << "\n";
        }

        return parse_result(parse_statement().unwrap());
    }

    parse_result parse_funcdecl() {
        token t0 = peek(0);
        token t1 = peek(1);
        token t2 = peek(2);
        ast_node node;
        ast_node args;
        ast_node body;

        if (t0.type == TOKEN_FUNCTION && t1.type == TOKEN_IDENTIFIER) {
            consume(2);
            node = ast_node(AST_FUNCDECL, t1.content);
            args = ast_node(AST_FUNCDECL_ARGS);
            body = ast_node(AST_FUNCDECL_BODY);

            if (t2.type != TOKEN_OPENPAREN) {
                return parse_result("'(' expected in function declaration");
            }

            consume(1);

            while ((t0 = peek(0)).type != TOKEN_CLOSEPAREN) {
                if (t0.type == TOKEN_EOF || t0.type == TOKEN_INVALID) {
                    return parse_result("')' expected in function declaration");
                }

                t1 = peek(1);
                
                if (t0.type == TOKEN_IDENTIFIER && t1.type == TOKEN_COLON) {
                    consume(2);
                    identifier_type type = parse_type();

                    if (type == TYPE_INVALID) {
                        return parse_result("invalid type in function declaration");
                    }

                    args.add_child(ast_node(AST_FUNCDECL_ARG, ast_node_info(t0.content, type)));
                }

                if ((t0 = peek(0)).type == TOKEN_COMMA) {
                    consume(1);
                    continue;
                } else if (t0.type != TOKEN_CLOSEPAREN) {
                    return parse_result("')' expected, found '" + t0.content + "' instead");
                }
            }

            if ((t0 = peek(0)).type != TOKEN_CLOSEPAREN) {
                return parse_result("')' expected in function declaration");
            }
            
            consume(1);

            if ((t0 = peek(0)).type != TOKEN_COLON) {
                return parse_result("':' expected in function declaration");
            }

            consume(1);
            identifier_type func_type = parse_type();

            if (func_type == TYPE_INVALID) {
                return parse_result("type expected after function declaration");
            }

            body.add_child(parse_block().unwrap());
            node.add_child(args);
            node.add_child(body);

            return parse_result(node);
        }

        return parse_result(parse_vardecl().unwrap());
    }

    ast_node parse() {
        return parse_vardecl().unwrap();
    }

    /*parse_result parse_funcdecl() {
        token t0 = peek(0);
        token t1 = peek(1);
        token t2 = peek(2);
        token ti;

        if (t0.type != TOKEN_FUNCTION) {
            return parse_result("'function' expected in function declaration");
        }

        if (t1.type != TOKEN_IDENTIFIER) {
            return parse_result("identifier expected in function declaration");
        }

        if (t2.type != TOKEN_OPENPAREN) {
            return parse_result("'(' expected in function declaration");
        }

        ast_node function_arguments = ast_node(AST_FUNCDECL_ARGS);

        consume(3);

        while ((ti = peek(0)).type != TOKEN_CLOSEPAREN) {
            if (ti.type == TOKEN_INVALID) {
                return parse_result("unterminated argument list in function declaration");
            }

            if (ti.type == TOKEN_COMMA) {
                consume(1);
                continue;
            }

            if (ti.type != TOKEN_IDENTIFIER) {
                return parse_result("identifier expected in argument list in function declaration");
            }

            token tc = peek(1);

            if (tc.type != TOKEN_COLON) {
                return parse_result("':' expected in arguments list in function declaration");
            }

            consume(2);
            identifier_type type = parse_type();

            if (type == TYPE_INVALID) {
                return parse_result("type expected in argument list in function declaration");
            }

            function_arguments.add_child(ast_node(AST_FUNCDECL_ARG, ast_node_info(ti.content, type)));
        } 

        if (ti.type != TOKEN_CLOSEPAREN) {
            return parse_result("')' expected in function declaration");
        }

        consume(1);
        t0 = peek(0);
        
        if (t0.type != TOKEN_COLON) {
            return parse_result("':' expected in function declaration");
        }

        consume(1);

        identifier_type func_type = parse_type();
        ast_node func_body = ast_node(AST_FUNCDECL_BODY);
        func_body.add_child(parse_statement().unwrap());
        ast_node function_declaration = ast_node(AST_FUNCDECL, ast_node_info(t1.content, func_type));
        
        function_declaration.add_child(function_arguments);
        function_declaration.add_child(func_body);

        return parse_result(function_declaration);
    }*/

    bool consume(std::size_t c) {
        if ((current_token + c) >= token_list.size()) {
            eof = true;
            return false;
        } else {
            current_token += c;
            return true;
        }
    }

    token peek(std::size_t c) {
        if ((current_token + c) >= token_list.size() || eof) {
            return token(TOKEN_EOF);
        } else {
            return token_list[current_token + c];
        }
    }

    /*parse_result parse_block() {
        token t0;

        while ((t0 = peek(0)).type != TOKEN_CLOSEBRACE) {
            parse_statement();
        }
        
        return parse_result(ast_node(AST_INVALID));
    }*/

    /*parse_result parse_for() { // too early :>
        token t0 = peek(0);
        token t1 = peek(1);

        if (t0.type != TOKEN_FOR) {
            return parse_result("'for' expected for for loop");
        }

        if (t1.type != TOKEN_OPENPAREN) {
            return parse_result("'(' expected for for loop");
        }

        consume(2);
        parse_statement(); // initialization

        if ((t0 = peek(0)).type != TOKEN_SEMICOLON) {
            return parse_result("';' expected for for loop");
        }

        parse_expression(); // condition

        if ((t0 = peek(0)).type != TOKEN_SEMICOLON) {
            return parse_result("';' expected for for loop");
        }

        parse_statement(); // iteration

        if ((t0 = peek(0)).type != TOKEN_CLOSEPAREN) {
            return parse_result("')' expected for for loop");
        }

        consume(1);
        while (1);
        parse_statement();

        while (1);
        return parse_result(ast_node(AST_INVALID));
    }*/

};

int main(int argc, char **argv)
{
    parser p;
    p.parse().print();
}
