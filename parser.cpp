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
        
        token_list.push_back(token(TOKEN_FUNCTION, "function"));
        token_list.push_back(token(TOKEN_IDENTIFIER, "main"));
        token_list.push_back(token(TOKEN_OPENPAREN, "("));
        token_list.push_back(token(TOKEN_IDENTIFIER, "args"));
        token_list.push_back(token(TOKEN_COLON, ":"));
        token_list.push_back(token(TOKEN_STRING, "string"));
        token_list.push_back(token(TOKEN_OPENBRACKET, "["));
        token_list.push_back(token(TOKEN_CLOSEBRACKET, "]"));
        token_list.push_back(token(TOKEN_CLOSEPAREN, ")"));
        token_list.push_back(token(TOKEN_COLON, ":"));
        token_list.push_back(token(TOKEN_INTEGER, "int"));
        token_list.push_back(token(TOKEN_OPENBRACE, "{"));
        token_list.push_back(token(TOKEN_IDENTIFIER, "print"));
        token_list.push_back(token(TOKEN_OPENPAREN, "("));
        token_list.push_back(token(TOKEN_LITERAL, "hello, world!"));
        token_list.push_back(token(TOKEN_CLOSEPAREN, ")"));
        token_list.push_back(token(TOKEN_SEMICOLON, ";"));
        token_list.push_back(token(TOKEN_FOR, "for"));
        token_list.push_back(token(TOKEN_OPENPAREN, "("));
        token_list.push_back(token(TOKEN_LET, "let"));
        token_list.push_back(token(TOKEN_IDENTIFIER, "i"));
        token_list.push_back(token(TOKEN_COLON, ":"));
        token_list.push_back(token(TOKEN_INTEGER, "int"));
        token_list.push_back(token(TOKEN_EQUAL, "="));
        token_list.push_back(token(TOKEN_NUMBER, "0"));
        token_list.push_back(token(TOKEN_SEMICOLON, ";"));
        token_list.push_back(token(TOKEN_IDENTIFIER, "i"));
        token_list.push_back(token(TOKEN_LESS_THAN, "<"));
        token_list.push_back(token(TOKEN_IDENTIFIER, "len"));
        token_list.push_back(token(TOKEN_OPENPAREN, "("));
        token_list.push_back(token(TOKEN_IDENTIFIER, "args"));
        token_list.push_back(token(TOKEN_CLOSEPAREN, ")"));
        token_list.push_back(token(TOKEN_SEMICOLON, ";"));
        token_list.push_back(token(TOKEN_IDENTIFIER, "i"));
        token_list.push_back(token(TOKEN_EQUAL, "="));
        token_list.push_back(token(TOKEN_IDENTIFIER, "i"));
        token_list.push_back(token(TOKEN_PLUS, "+"));
        token_list.push_back(token(TOKEN_NUMBER, "1"));
        token_list.push_back(token(TOKEN_CLOSEPAREN, ")"));
        token_list.push_back(token(TOKEN_OPENBRACE, "{"));
        token_list.push_back(token(TOKEN_IDENTIFIER, "print"));
        token_list.push_back(token(TOKEN_OPENPAREN, "("));
        token_list.push_back(token(TOKEN_IDENTIFIER, "args"));
        token_list.push_back(token(TOKEN_OPENBRACKET, "["));
        token_list.push_back(token(TOKEN_IDENTIFIER, "i"));
        token_list.push_back(token(TOKEN_CLOSEBRACKET, "]"));
        token_list.push_back(token(TOKEN_CLOSEPAREN, ")"));
        token_list.push_back(token(TOKEN_SEMICOLON, ";"));
        token_list.push_back(token(TOKEN_CLOSEBRACE, "}"));
        token_list.push_back(token(TOKEN_RETURN, "return"));
        token_list.push_back(token(TOKEN_IDENTIFIER, "len"));
        token_list.push_back(token(TOKEN_OPENPAREN, "("));
        token_list.push_back(token(TOKEN_IDENTIFIER, "args"));
        token_list.push_back(token(TOKEN_CLOSEPAREN, ")"));
        token_list.push_back(token(TOKEN_PLUS, "+"));
        token_list.push_back(token(TOKEN_NUMBER, "1"));
        token_list.push_back(token(TOKEN_SEMICOLON, ";"));
        token_list.push_back(token(TOKEN_CLOSEBRACE, "}"));
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

    parse_result parse_primary() {
        std::cout << "parse_primary\n";
        token t0 = peek(0);

        switch (t0.type) {
        case TOKEN_IDENTIFIER: {
            std::string identifier = t0.content;
            consume(1);
            ast_node node;

            if ((t0 = peek(0)).type == TOKEN_OPENBRACKET) {
                consume(1);
                node = ast_node(AST_ARRAY_ACCESS);
                node.add_child(parse_expression().unwrap());
                if ((t0 = peek(0)).type != TOKEN_CLOSEBRACKET) {
                    return parse_result("']' expected");
                }

                consume(1);
            } else {
                node = ast_node(AST_IDENTIFIER, ast_node_info(identifier));
            }

            return parse_result(node); 
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

    parse_result parse_call() {
        std::cout << "parse_call\n";
        token t0 = peek(0);
        ast_node primary = parse_primary().unwrap();

        while ((t0 = peek(0)).type == TOKEN_OPENPAREN) {
            std::cout << "Function call: " + peek(2).content << "\n";
            consume(1);

            ast_node funccall = ast_node(AST_FUNCCALL);
            ast_node callee = ast_node(AST_FUNCCALL_CLLE);
            ast_node arguments = ast_node(AST_FUNCCALL_ARGS);

            while ((t0 = peek(0)).type != TOKEN_CLOSEPAREN) {
                arguments.add_child(parse_expression().unwrap());
                std::cout << t0.content << "\n"; 

                if ((t0 = peek(0)).type == TOKEN_COMMA) {
                    consume(1);
                    continue;
                } else if (t0.type == TOKEN_CLOSEPAREN) {
                    consume(1);
                    break;
                } else {
                    return parse_result("',' or ')' expected for function call arguments " + t0.content);
                }
            }

            callee.add_child(primary);
            funccall.add_child(callee);
            funccall.add_child(arguments);

            primary = funccall;
        }

        return parse_result(primary);
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

        return parse_result(parse_call().unwrap());
    }

    parse_result parse_factor() {
        std::cout << "parse_factor\n";
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

    parse_result parse_and() {
        token t0;
        ast_node node = parse_equality().unwrap();

        if ((t0 = peek(0)).type == TOKEN_AND) {
            ast_node and_node = ast_node(AST_AND);
            and_node.add_child(node);

            while ((t0 = peek(0)).type == TOKEN_AND) {
                consume(1);
                and_node.add_child(parse_equality().unwrap());
            }

            return parse_result(and_node);
        }

        return parse_result(node);
    }

    parse_result parse_or() {
        token t0;
        ast_node node = parse_and().unwrap();

        if ((t0 = peek(0)).type == TOKEN_OR) {
            ast_node or_node = ast_node(AST_OR);
            or_node.add_child(node);

            while ((t0 = peek(0)).type == TOKEN_OR) {
                consume(1);
                or_node.add_child(parse_and().unwrap());
            }

            return parse_result(or_node);
        }

        return parse_result(node);
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

        return parse_result(parse_or().unwrap());
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

    parse_result parse_block() {
        token t0 = peek(0);
        ast_node block;
        std::vector<ast_node> block_stmts;

        if (t0.type != TOKEN_OPENBRACE) {
            return parse_result("Invalid block");
        }

        consume(1);

        while ((t0 = peek(0)).type != TOKEN_CLOSEBRACE) {
            std::cout << "ciuga\n";
            block_stmts.push_back(parse_vardecl().unwrap());
        }

        consume(1);
        block.add_children(block_stmts);
        
        return parse_result(block);
    }

    parse_result parse_if() {
        std::cout << "parse_if()\n";
        token t0 = peek(0);
        ast_node node = ast_node(AST_IF);
        ast_node condition = ast_node(AST_CONDITION);
        ast_node body = ast_node(AST_IF_BODY);
        ast_node else_body = ast_node(AST_IF_ELSE);

        if (t0.type != TOKEN_IF) {
            return parse_result("Invalid if");
        }

        consume(1);

        if ((t0 = peek(0)).type != TOKEN_OPENPAREN) {
            return parse_result("'(' expected for if statement");
        }

        consume(1);

        condition.add_child(parse_expression().unwrap());

        if ((t0 = peek(0)).type != TOKEN_CLOSEPAREN) {
            return parse_result("')' expected for if statement");
        }

        consume(1);

        body.add_child(parse_statement().unwrap());

        node.add_child(condition);
        node.add_child(body);

        if ((t0 = peek(0)).type == TOKEN_ELSE) {
            consume(1);
            else_body.add_child(parse_statement().unwrap());
            node.add_child(else_body);
        }

        return node;
    }

    parse_result parse_while() {
        std::cout << "parse_while()\n";
        token t0 = peek(0);
        ast_node node = ast_node(AST_WHILE);
        ast_node body = ast_node(AST_WHILE_BODY);
        ast_node condition = ast_node(AST_CONDITION);

        if (t0.type != TOKEN_WHILE) {
            return parse_result("Invalid while");
        }

        consume(1);

        if ((t0 = peek(0)).type != TOKEN_OPENPAREN) {
            return parse_result("'(' expected for while statement");
        }

        consume(1);

        condition.add_child(parse_expression().unwrap());

        if ((t0 = peek(0)).type != TOKEN_CLOSEPAREN) {
            return parse_result("')' expected for while statement");
        }

        consume(1);

        body.add_child(parse_statement().unwrap());
        node.add_child(condition);
        node.add_child(body);

        return parse_result(node);
    }

    parse_result parse_for() {
        std::cout << "parse_for()\n";
        token t0;
        ast_node node = ast_node(AST_FOR);
        ast_node init = ast_node(AST_FOR_INIT);
        ast_node step = ast_node(AST_FOR_STEP);
        ast_node body = ast_node(AST_FOR_BODY);
        ast_node condition = ast_node(AST_CONDITION);

        if ((t0 = peek(0)).type != TOKEN_FOR) {
            std::cout << t0.content << "\n";
            return parse_result("Invalid for");
        }

        consume(1);

        if ((t0 = peek(0)).type != TOKEN_OPENPAREN) {
            return parse_result("'(' expected for for statement");
        }

        consume(1);

        if ((t0 = peek(0)).type != TOKEN_SEMICOLON) {
            if (t0.type == TOKEN_LET) {
                init.add_child(parse_vardecl().unwrap());
            } else {
                init.add_child(parse_exprstmt().unwrap());
            }
        } else {
            consume(1);
        }

        if ((t0 = peek(0)).type != TOKEN_SEMICOLON) {
            condition.add_child(parse_expression().unwrap());
        }

        if ((t0 = peek(0)).type != TOKEN_SEMICOLON) {
            return parse_result("';' expected after for condition expression");
        }
        
        consume(1);

        if ((t0 = peek(0)).type != TOKEN_CLOSEPAREN) {
            step.add_child(parse_expression().unwrap());
        }

        if ((t0 = peek(0)).type != TOKEN_CLOSEPAREN) {
            return parse_result("')' expected for for statement");
        }
        
        consume(1);

        body.add_child(parse_statement().unwrap());
        node.add_child(init);
        node.add_child(condition);
        node.add_child(step);
        node.add_child(body);

        return parse_result(node);
    }

    parse_result parse_return() {
        std::cout << "parse_return\n";
        token t0;

        if ((t0 = peek(0)).type != TOKEN_RETURN) {
            return parse_result("Invalid return");
        }

        consume(1);
        ast_node node = ast_node(AST_RETURN);

        if ((t0 = peek(0)).type != TOKEN_SEMICOLON) {
            node.add_child(parse_expression().unwrap());

            if ((t0 = peek(0)).type != TOKEN_SEMICOLON) {
                return parse_result("';' expected after return statement");
            }
        }

        consume(1);
        return parse_result(node);
    }

    parse_result parse_statement() {
        std::cout << "parse_statement()\n";
        token t0 = peek(0);

        switch (t0.type) {
        case TOKEN_OPENBRACE:
            return parse_result(parse_block().unwrap());
        case TOKEN_IF:
            return parse_result(parse_if().unwrap());
        case TOKEN_WHILE:
            return parse_result(parse_while().unwrap());
        case TOKEN_FOR:
            return parse_result(parse_for().unwrap());
        case TOKEN_RETURN:
            return parse_result(parse_return().unwrap());
        default:
            break;
        }

        return parse_result(parse_exprstmt().unwrap());
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
        } else if (t0.type == TOKEN_FUNCTION) {
            consume(1);

            if ((t0 = peek(0)).type != TOKEN_IDENTIFIER) {
                return parse_result("identifier expected for function declaration");
            }

            consume(1);

            if ((t0 = peek(0)).type != TOKEN_OPENPAREN) {
                return parse_result("'(' expected for function declaration");
            }

            consume(1);

            ast_node arguments = ast_node(AST_FUNCDECL_ARGS);

            while ((t0 = peek(0)).type != TOKEN_INVALID && t0.type != TOKEN_EOF) {
                std::string name = t0.content;
                identifier_type type;

                if ((t0 = peek(0)).type != TOKEN_IDENTIFIER) {
                    return parse_result("identifier expected for function argument");
                }

                consume(1);

                if ((t0 = peek(0)).type != TOKEN_COLON) {
                    return parse_result("':' expected for function argument");
                }
                
                consume(1);

                if ((type = parse_type()) == TYPE_INVALID) {
                    return parse_result("type expected for function argument");
                }

                std::cout << "addo " << name << "\n";
                arguments.add_child(ast_node(AST_FUNCDECL_ARG, ast_node_info(name, type)));

                if ((t0 = peek(0)).type == TOKEN_COMMA) {
                    consume(1);
                    continue;
                } else if (t0.type == TOKEN_CLOSEPAREN) {
                    consume(1);
                    break;
                } else {
                    return parse_result("',' or ')' expected for function declaration");
                }
            }

            if ((t0 = peek(0)).type != TOKEN_COLON) {
                return parse_result("':' expected for function declaration");
            }

            consume(1);

            identifier_type func_type = parse_type();

            if (func_type == TYPE_INVALID) {
                return parse_result("type expected for function declaration");
            }

            ast_node body = ast_node(AST_FUNCDECL_BODY);
            body.add_child(parse_statement().unwrap());

            ast_node funcdecl = ast_node(AST_FUNCDECL);
            funcdecl.add_child(arguments);
            funcdecl.add_child(body);

            return parse_result(funcdecl);
        }

        return parse_result(parse_statement().unwrap());
    }

    ast_node parse() {
        token t0;
        std::vector<ast_node> stmts;
        ast_node program = ast_node(AST_PROGRAM);

        while ((t0 = peek(0)).type != TOKEN_EOF && t0.type != TOKEN_INVALID) {
            stmts.push_back(parse_vardecl().unwrap());
        }
        
        program.add_children(stmts);
        return program;
    }


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

};

int main(int argc, char **argv)
{
    parser p;
    p.parse().print();
}
