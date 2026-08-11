#pragma once

#include "token.h"
#include <string>
#include <vector>

class Lexer {
private:
    std::string source_;
    std::vector<Token> tokens_;
    
    size_t start_ = 0;
    size_t current_ = 0;

    bool IsAtEnd() const;
    char Advance();
    char Peek() const;
    char PeekNext() const;
    
    void AddToken(TokenType type);
    void ScanToken();
    void Number();
    void Identifier();

    bool IsDigit(char c) const;
    bool IsAlpha(char c) const;

public:
    explicit Lexer(const std::string& source);
    std::vector<Token> Tokenize();
};