#include "token.hpp"

token::token() {
    this->type = TOKEN_INVALID;
}

token::token(token_type type) {
    this->type = type;
}

token::token(token_type type, std::string content) {
    this->type = type;
    this->content = content;
}

void token::print() {
    std::cout << content << "\n";
}