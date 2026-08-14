#pragma once

#include "expressions/expression.h"
#include "expressions/terminal/coord_expression.h"
#include "expressions/terminal/number_expression.h"
#include "expressions/terminal/string_expression.h"
#include "expressions/nonterminal/configure.h"
#include "expressions/terminal/clear_expression.h"
#include "expressions/terminal/quit_expression.h"

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

        NumberExpression* ParseNumber(); 
        StringExpression* ParseString(); 
        CoordExpression* ParseVector();

        Expression* ParseAnalyzeCommand();
        ConfigureExpression* ParseConfigureCommand();
        ClearExpression* ParseClearCommand();
        QuitExpression* ParseQuitCommand();

        bool configure_mode_;

    public:
        explicit Parser(const std::vector<Token>& tokens, bool config_mode = false);
        Expression* Parse(); 
};