import 'dart:io';
import 'dart:developer';

enum TokenType {
    Invalid,
    Function,
    Let,
    Literal,
    Number,
    Type,
    Identifier,
    Equal,
    Comma,
    Colon,
    LessThan,
    GreaterThan,
    Semicolon,
    OpenParen,
    CloseParen,
    OpenBracket,
    CloseBracket,
    OpenBrace,
    CloseBrace,
    Quote,
    Dot,
    For,
    Return,
    While,
    If,
    Plus,
    Minus,
    Incr,
}

class Token {
    final TokenType type;
    final String content;

    Token(this.type, this.content);
}

class TokenList {
    int currentIndex = 0;
    final List<Token> list;

    TokenList(this.list);

    Token next() {
        if (currentIndex == list.length) {
            return Token(TokenType.Invalid, '');
        }

        return list[currentIndex++];
    }

    Token peek(int index) {
        if ((currentIndex + index) >= list.length) {
            return Token(TokenType.Invalid, '');
        }

        return list[currentIndex + index];
    }

    bool advance(int count) {
        if ((currentIndex + count) >= list.length) {
            return false;
        }

        currentIndex += count;
        return true;
    }

    int getIndex() => currentIndex;

    print() {
        for (int i = 0; i < list.length; i++) {
            stdout.write(list[i].content);
        }
    }
}

enum NodeType {
    Invalid,
    Block,
    Statement,
    Expression,
}

class ASTNode {
    final String name;
    final NodeType type;
    List<ASTNode> children = <ASTNode>[];

    ASTNode(this.name, this.type);

    void addChild(ASTNode child) {
        children.add(child);
    }

    ASTNode getChildByIndex(int index) => children[index];

    ASTNode getChildByName(String name) {
        for (int i = 0; i < children.length; i++) {
            if (name == this.name) {
                return children[i];
            }
        }

        return ASTNode('', NodeType.Invalid);
    }
}

enum ResultType {
    Ok,
    Error,
}

class Result {
    final ResultType type;
    final String message;
    final int count;
    
    Result(this.type, this.message, this.count);
}

Result parseFunctionDefinition(TokenList list) {
    int startIndex = list.getIndex();
    Token currentToken;

    final Token identifier = list.next();
    final Token openParen = list.next();

    if (identifier.type != TokenType.Identifier) {
        return Result(ResultType.Error, 'expected identifier for function definition', 0);
    }

    if (openParen.type != TokenType.OpenParen) {
        return Result(ResultType.Error, 'expected "(" for function definition', 0);
    }

    while ((currentToken = list.next()).type != TokenType.CloseParen) {  
        if (currentToken.type == TokenType.Invalid) {
            return Result(ResultType.Error, 'expected ")" for function definition', 0);
        }

        if (currentToken.type == TokenType.Comma) {
            continue;
        }

        final Token argIdentifier = currentToken;
        final Token argColon = list.next();
        final Token argType = list.next();

        if (argIdentifier.type != TokenType.Identifier) {
            return Result(ResultType.Error, 'expected identifier in argument in function definition', 0);
        }

        if (argColon.type != TokenType.Colon) {
            return Result(ResultType.Error, 'expected ":" in argument in function definition', 0);
        }

        if (argType.type != TokenType.Type) {
            return Result(ResultType.Error, 'expected type in argument in function definition', 0);
        }

        print('${argIdentifier.content}, ${argType.content}');
    }

    return Result(ResultType.Ok, '', list.getIndex() - startIndex);
}

void main() {
    List<Token> tokens = <Token>[
        Token(TokenType.Function, 'function'),
        Token(TokenType.Identifier, 'main'),
        Token(TokenType.OpenParen, '('),
        Token(TokenType.Identifier, 'args'),
        Token(TokenType.Colon, ':'),
        Token(TokenType.Type, 'string'),
        Token(TokenType.OpenBracket, '['),
        Token(TokenType.CloseBracket, ']'),
        Token(TokenType.CloseParen, ')'),
        Token(TokenType.Colon, ':'),
        Token(TokenType.Type, 'int'),
        Token(TokenType.OpenBrace, '{'),
        Token(TokenType.Identifier, 'print'),
        Token(TokenType.OpenParen, '('),
        Token(TokenType.Literal, 'hello, world!'),
        Token(TokenType.CloseParen, ')'),
        Token(TokenType.Semicolon, ';'),
        Token(TokenType.For, 'for'),
        Token(TokenType.OpenParen, '('),
        Token(TokenType.Let, 'let'),
        Token(TokenType.Identifier, 'i'),
        Token(TokenType.Colon, ':'),
        Token(TokenType.Type, 'int'),
        Token(TokenType.Equal, '='),
        Token(TokenType.Number, '0'),
        Token(TokenType.Semicolon, ';'),
        Token(TokenType.Identifier, 'i'),
        Token(TokenType.LessThan, '<'),
        Token(TokenType.Identifier, 'args'),
        Token(TokenType.Dot, '.'),
        Token(TokenType.Identifier, 'length'),
        Token(TokenType.Semicolon, ';'),
        Token(TokenType.Identifier, 'i'),
        Token(TokenType.Incr, '++'),
        Token(TokenType.CloseParen, ')'),
        Token(TokenType.OpenBrace, '{'),
        Token(TokenType.Identifier, 'print'),
        Token(TokenType.OpenParen, '('),
        Token(TokenType.Literal, 'arg %d: %s'),
        Token(TokenType.Comma, ','),
        Token(TokenType.OpenBracket, '['),
        Token(TokenType.Identifier, 'i'),
        Token(TokenType.Comma, ','),
        Token(TokenType.Identifier, 'args'),
        Token(TokenType.OpenBracket, '['),
        Token(TokenType.Identifier, 'i'),
        Token(TokenType.CloseBracket, ']'),
        Token(TokenType.CloseBracket, ']'),
        Token(TokenType.CloseParen, ')'),
        Token(TokenType.Semicolon, ';'),
        Token(TokenType.CloseBrace, '}'),
        Token(TokenType.Return, 'return'),
        Token(TokenType.Number, '0'),
        Token(TokenType.Semicolon, ';'),
        Token(TokenType.CloseBrace, '}'),
    ];

    TokenList list = TokenList(tokens);
    Token currentToken;
    
    ASTNode ast = ASTNode("Program", NodeType.Block);

    while ((currentToken = list.next()).type != TokenType.Invalid) {
        switch (currentToken.type) {
            case TokenType.Function: {
                Result res = parseFunctionDefinition(list);
                inspect(res);
            }
            break;
        }
    }

}