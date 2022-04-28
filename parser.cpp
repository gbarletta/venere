#include <iostream>
#include <vector>

enum token_type {
    TOKEN_INVALID = -1,
    TOKEN_FUNCTION,
    TOKEN_LET,
    TOKEN_LITERAL,
    TOKEN_NUMBER,
    TOKEN_TYPE,
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
    AST_IDENTIFIER,
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
};

class ast_node_info {
public:
    int number;
    std::string literal;
    std::string identifier;
    

    ast_node_info() {
        this->identifier = "";
    }
    
    ast_node_info(std::string str) {
        this->identifier = str;
        this->literal = str;
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
    std::size_t current_token;
    std::vector<token> token_list;

    parser() {
        current_token = 0;
        token_list.push_back(token(TOKEN_OPENPAREN, "("));
        token_list.push_back(token(TOKEN_IDENTIFIER, "i"));
        token_list.push_back(token(TOKEN_STAR, "*"));
        token_list.push_back(token(TOKEN_NUMBER, "69"));
        token_list.push_back(token(TOKEN_CLOSEPAREN, ")"));
        token_list.push_back(token(TOKEN_DASH, "-"));
        token_list.push_back(token(TOKEN_NUMBER, "3"));
        token_list.push_back(token(TOKEN_STAR, "*"));
        token_list.push_back(token(TOKEN_NUMBER, "5"));
        token_list.push_back(token(TOKEN_PLUS, "+"));
        token_list.push_back(token(TOKEN_OPENPAREN, "("));
        token_list.push_back(token(TOKEN_IDENTIFIER, "j"));
        token_list.push_back(token(TOKEN_PLUS, "+"));
        token_list.push_back(token(TOKEN_IDENTIFIER, "k"));
        token_list.push_back(token(TOKEN_PLUS, "+"));
        token_list.push_back(token(TOKEN_OPENPAREN, "("));
        token_list.push_back(token(TOKEN_NUMBER, "23"));
        token_list.push_back(token(TOKEN_DASH, "-"));
        token_list.push_back(token(TOKEN_NUMBER, "21"));
        token_list.push_back(token(TOKEN_STAR, "*"));
        token_list.push_back(token(TOKEN_NUMBER, "5"));
        token_list.push_back(token(TOKEN_CLOSEPAREN, ")"));
        token_list.push_back(token(TOKEN_PLUS, "+"));
        token_list.push_back(token(TOKEN_NUMBER, "3"));
        token_list.push_back(token(TOKEN_SLASH, "/"));
        token_list.push_back(token(TOKEN_NUMBER, "2"));
        token_list.push_back(token(TOKEN_CLOSEPAREN, ")"));
    }

    parse_result parse_primary() {
        token t0 = peek(0);

        switch (t0.type) {
        case TOKEN_IDENTIFIER:
            consume(1);
            return parse_result(ast_node(AST_IDENTIFIER, ast_node_info(t0.content)));
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
        token t0 = peek(0);

        if (t0.type == TOKEN_EXCL || t0.type == TOKEN_DASH) {
            consume(1);
            ast_node node = (t0.type == TOKEN_EXCL) ? ast_node(AST_NEGATE) : ast_node(AST_MINUS);
            node.add_child(parse_unary().unwrap());
            return parse_result(node);
        } 

        return parse_result(parse_primary().unwrap());
    }

    parse_result parse_factor() {
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

    parse_result parse_expression() {
        return parse_result(parse_equality().unwrap());
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
            return token(TOKEN_INVALID);
        } else {
            return token_list[current_token + c];
        }
    }

    ast_node parse() {
        parse_expression().unwrap().print();
        return ast_node(AST_INVALID);
    }

    parse_result parse_type() {
        token t0 = peek(0);
        token t1 = peek(1);
        token t2 = peek(2);

        if (t0.type != TOKEN_TYPE) {
            return parse_result("Not a type");
        }

        if (t1.type == TOKEN_OPENBRACKET && t2.type == TOKEN_CLOSEBRACKET) {
            std::cout << "Array of " << t0.content << "\n";
            consume(3);
        } else {
            std::cout << t0.content << "\n";
            consume(1);
        }

        std::cout << "end type\n";
        return parse_result(ast_node(AST_INVALID));
    }

    parse_result parse_block() {
        token t0;

        while ((t0 = peek(0)).type != TOKEN_CLOSEBRACE) {
            parse_statement();
        }
        
        return parse_result(ast_node(AST_INVALID));
    }

    parse_result parse_assignment() {
        return parse_result(ast_node(AST_INVALID));
    }

    parse_result parse_funccall() {
        token t0 = peek(0);
        token t1 = peek(1);
        token ti;

        if (t0.type != TOKEN_IDENTIFIER) {
            return parse_result("Identifier expected for function call");
        }

        if (t1.type != TOKEN_OPENPAREN) {
            return parse_result("'(' expected for function call");
        }

        consume(2);

        while ((ti = peek(0)).type != TOKEN_CLOSEPAREN) {
            if (ti.type == TOKEN_INVALID) {
                return parse_result("')' expected for function call");
            }

            parse_expression();  
            t0 = peek(0);

            if (t0.type != TOKEN_COMMA) {
                break;
            }

            consume(1);     
        }     

        if (t0.type != TOKEN_CLOSEPAREN) {
            return parse_result("')' expected for function call");
        }

        consume(1);
        return parse_result(ast_node(AST_INVALID));
    }

    parse_result parse_for() { // too early :>
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
    }

    parse_result parse_statement() {
        token t0 = peek(0);
        token t1 = peek(1);

        std::cout << "parsing statement\n";

        /*switch (t0.type) {
        case TOKEN_OPENBRACE:
            consume(1);
            parse_block();
            break;
        case TOKEN_IDENTIFIER:
            if (t1.type == TOKEN_EQUAL) {
                parse_assignment();
            } else if (t1.type == TOKEN_OPENPAREN) {
                parse_funccall();
            }
            break;
        case TOKEN_FOR:
            parse_for();
            while (1);
            break;
        case TOKEN_SEMICOLON:
            std::cout << "NOP!\n";
            consume(1);
            break;
        case TOKEN_LET:
            std::cout << "LET!\n";
            break;
        default:
            break;
        }

        std::cin.get();
        std::cout << "end statement\n";*/
        return parse_result(ast_node(AST_INVALID));
    }

    parse_result parse_funcdecl() {
        token t0 = peek(0);
        token t1 = peek(1);
        token t2 = peek(2);
        token ti;

        if (t0.type != TOKEN_FUNCTION) {
            return parse_result("Not a function");
        }

        if (t1.type != TOKEN_IDENTIFIER) {
            return parse_result("Identifier expected in function declaration");
        }

        if (t2.type != TOKEN_OPENPAREN) {
            return parse_result("'(' expected in function declaration");
        }

        consume(3);

        while ((ti = peek(0)).type != TOKEN_CLOSEPAREN) {
            if (ti.type == TOKEN_INVALID) {
                return parse_result("Unterminated arguments list in function declaration");
            }

            if (ti.type == TOKEN_COMMA) {
                consume(1);
                continue;
            }

            if (ti.type != TOKEN_IDENTIFIER) {
                return parse_result("Identifier expected in arguments list in function declaration");
            }

            std::cout << "Param: " << ti.content << " is a ";
            token tc = peek(1);

            if (tc.type != TOKEN_COLON) {
                return parse_result("':' expected in arguments list in function declaration");
            }

            consume(2);
            parse_type();
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
        parse_type();
        parse_statement();
        
        return parse_result(ast_node(AST_INVALID));
    }
};

int main(int argc, char **argv)
{
    parser p;
    p.parse();
}
