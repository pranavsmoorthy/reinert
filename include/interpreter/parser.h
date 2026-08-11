#pragma once

#include "expression.h"
#include "expressions/terminal/coord_expression.h"
#include "expressions/nonterminal/analyze_manual.h"
#include "token.h"

#include <vector>
#include <string>
#include <stdexcept>

class Parser {
    private:
        std::vector<Token> tokens_;
        size_t current_ = 0;

        Token Peek() const;
        Token Advance();
        bool IsAtEnd() const;
        bool Check(TokenType type) const;
        
        Token Consume(TokenType type, const std::string& message);

        CoordExpression* ParseVector();
        Expression* ParseAnalyzeCommand();

    public:
        explicit Parser(const std::vector<Token>& tokens);
        Expression* Parse(); 
};