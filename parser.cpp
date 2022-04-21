#include <iostream>
#include <vector>

enum token_type {
    INVALID_TOKEN = -1,
    FUNCTION,
    LET,
    LITERAL,
    NUMBER,
    TYPE,
    IDENTIFIER,
    EQUAL,
    COMMA,
    COLON,
    LESSTHAN,
    GREATERTHAN,
    SEMICOLON,
    OPENPAREN,
    CLOSEPAREN,
    OPENBRACKET,
    CLOSEBRACKET,
    OPENBRACE,
    CLOSEBRACE,
    QUOTE,
    DOT,
    FOR,
    RETURN,
    WHILE,
    IF,
    PLUS,
    MINUS,
    INCR,
};

class token {
public:
    token_type type;
    std::string content;

    token() {
        this->type = INVALID_TOKEN;
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
    INVALID_AST = -1,
    BLOCK,
    STATEMENT,
    EXPRESSION
};

class ast_node_info {
private:

};

class ast_node {
public:
    ast_type type;
    ast_node_info info;
    std::vector<ast_node> children;

    ast_node() {
        this->type = BLOCK;
    }
    
    ast_node(ast_type type) {
        this->type = type;
    }

    ast_node(ast_type type, ast_node_info info) {
        this->type = type;
        this->info = info;
    }

    void add_child(ast_node child) {
        this->children.push_back(child);
    }
};

class parse_result {
public:
    ast_node node;
    std::string error;

    parse_result(std::string error) {
        node = ast_node(INVALID_AST);
        this->error = error;
    }

    parse_result(ast_node node) {
        this->node = node;
    }
};

class parser {
public:
    std::size_t current_token;
    std::vector<token> token_list;

    parser() {
        current_token = 0;
        token_list.push_back(token(FUNCTION, "function"));
        token_list.push_back(token(IDENTIFIER, "main"));
        token_list.push_back(token(OPENPAREN, "("));
        token_list.push_back(token(IDENTIFIER, "args"));
        token_list.push_back(token(COLON, ":"));
        token_list.push_back(token(TYPE, "string"));
        token_list.push_back(token(OPENBRACKET, "["));
        token_list.push_back(token(CLOSEBRACKET, "]"));
        token_list.push_back(token(CLOSEPAREN, ")"));
        token_list.push_back(token(COLON, ":"));
        token_list.push_back(token(TYPE, "int"));
        token_list.push_back(token(OPENBRACE, "{"));
        token_list.push_back(token(IDENTIFIER, "print"));
        token_list.push_back(token(OPENPAREN, "("));
        token_list.push_back(token(LITERAL, "hello, world!"));
        token_list.push_back(token(CLOSEPAREN, ")"));
        token_list.push_back(token(SEMICOLON, ";"));
        token_list.push_back(token(FOR, "for"));
        token_list.push_back(token(OPENPAREN, "("));
        token_list.push_back(token(LET, "let"));
        token_list.push_back(token(IDENTIFIER, "i"));
        token_list.push_back(token(COLON, ":"));
        token_list.push_back(token(TYPE, "int"));
        token_list.push_back(token(EQUAL, "="));
        token_list.push_back(token(NUMBER, "0"));
        token_list.push_back(token(SEMICOLON, ";"));
        token_list.push_back(token(IDENTIFIER, "i"));
        token_list.push_back(token(LESSTHAN, "<"));
        token_list.push_back(token(IDENTIFIER, "args"));
        token_list.push_back(token(DOT, "."));
        token_list.push_back(token(IDENTIFIER, "length"));
        token_list.push_back(token(SEMICOLON, ";"));
        token_list.push_back(token(IDENTIFIER, "i"));
        token_list.push_back(token(INCR, "++"));
        token_list.push_back(token(CLOSEPAREN, ")"));
        token_list.push_back(token(OPENBRACE, "{"));
        token_list.push_back(token(IDENTIFIER, "print"));
        token_list.push_back(token(OPENPAREN, "("));
        token_list.push_back(token(LITERAL, "arg %d: %s"));
        token_list.push_back(token(COMMA, ","));
        token_list.push_back(token(OPENBRACKET, "["));
        token_list.push_back(token(IDENTIFIER, "i"));
        token_list.push_back(token(COMMA, ","));
        token_list.push_back(token(IDENTIFIER, "args"));
        token_list.push_back(token(OPENBRACKET, "["));
        token_list.push_back(token(IDENTIFIER, "i"));
        token_list.push_back(token(CLOSEBRACKET, "]"));
        token_list.push_back(token(CLOSEBRACKET, "]"));
        token_list.push_back(token(CLOSEPAREN, ")"));
        token_list.push_back(token(SEMICOLON, ";"));
        token_list.push_back(token(CLOSEBRACE, "}"));
        token_list.push_back(token(RETURN, "return"));
        token_list.push_back(token(NUMBER, "0"));
        token_list.push_back(token(SEMICOLON, ";"));
        token_list.push_back(token(CLOSEBRACE, "}"));
    }

    bool consume(std::size_t c) {
        if ((current_token + c) >= token_list.size()) {
            return false;
        } else {
            current_token += c;
            return true;
        }
    }

    token peek(std::size_t c) {
        if ((current_token + c) >= token_list.size()) {
            return token(INVALID_TOKEN);
        } else {
            return token_list[current_token + c];
        }
    }

    ast_node parse() {
        token t;

        while ((t = peek(0)).type != INVALID_TOKEN) {
            if (t.type == FUNCTION) {
                parse_function();
            }
        }

        return ast_node(INVALID_AST);
    }

    parse_result parse_type() {
        token t0 = peek(0);
        token t1 = peek(1);
        token t2 = peek(2);

        if (t0.type != TYPE) {
            return parse_result("Not a type");
        }

        if (t1.type == OPENBRACKET && t2.type == CLOSEBRACKET) {
            std::cout << "Array of " << t0.content << "\n";
            consume(3);
        } else {
            std::cout << t0.content << "\n";
            consume(1);
        }

        return parse_result(ast_node(INVALID_AST));
    }

    parse_result parse_block() {
        parse_statement();
        return parse_result(ast_node(INVALID_AST));
    }

    parse_result parse_assignment() {
        return parse_result(ast_node(INVALID_AST));
    }

    parse_result parse_expression() {
        token t0 = peek(0);

        if (t0.type == LITERAL) {
            consume(1);
            std::cout << "LiteralExpr\n";
            return parse_result(ast_node(INVALID_AST));
        } else if (t0.type == IDENTIFIER) {
            consume(1);
            std::cout << "IdentifierExpr\n";
            return parse_result(ast_node(INVALID_AST));
        }
        
        return parse_result("Invalid expression");
    }

    parse_result parse_funccall() {
        token t0 = peek(0);
        token t1 = peek(1);
        token ti;

        if (t0.type != IDENTIFIER) {
            return parse_result("Identifier expected for function call");
        }

        if (t1.type != OPENPAREN) {
            return parse_result("'(' expected for function call");
        }

        consume(2);

        while ((ti = peek(0)).type != CLOSEPAREN) {
            if (ti.type == INVALID_TOKEN) {
                return parse_result("')' expected for function call");
            }

            parse_expression();  
            t0 = peek(0);

            if (t0.type != COMMA) {
                break;
            }

            consume(1);     
        }     

        if (t0.type != CLOSEPAREN) {
            return parse_result("')' expected for function call");
        }

        return parse_result(ast_node(INVALID_AST));
    }

    parse_result parse_statement() {
        token t0 = peek(0);
        token t1 = peek(1);

        switch (t0.type) {
        case OPENBRACE:
            consume(1);
            parse_block();
            break;
        case IDENTIFIER:
            if (t1.type == EQUAL) {
                parse_assignment();
            } else if (t1.type == OPENPAREN) {
                parse_funccall();
            }
            break;
        }

        return parse_result(ast_node(INVALID_AST));
    }

    parse_result parse_function() {
        token t0 = peek(0);
        token t1 = peek(1);
        token t2 = peek(2);
        token ti;

        if (t0.type != FUNCTION) {
            return parse_result("Not a function");
        }

        if (t1.type != IDENTIFIER) {
            return parse_result("Identifier expected in function declaration");
        }

        if (t2.type != OPENPAREN) {
            return parse_result("'(' expected in function declaration");
        }

        consume(3);

        while ((ti = peek(0)).type != CLOSEPAREN) {
            if (ti.type == INVALID_TOKEN) {
                return parse_result("Unterminated arguments list in function declaration");
            }

            if (ti.type != IDENTIFIER) {
                return parse_result("Identifier expected in arguments list in function declaration");
            }

            std::cout << "Param: " << ti.content << " is a ";
            token tc = peek(1);

            if (tc.type != COLON) {
                return parse_result("':' expected in arguments list in function declaration");
            }

            consume(2);
            parse_type();
        } 

        if (ti.type != CLOSEPAREN) {
            return parse_result("')' expected in function declaration");
        }

        consume(1);
        t0 = peek(0);
        
        if (t0.type != COLON) {
            return parse_result("':' expected in function declaration");
        }

        consume(1);
        parse_type();
        parse_statement();
    }
};

int main(int argc, char **argv)
{
    parser p;
    p.parse();
}
