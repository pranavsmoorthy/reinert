#pragma once

#include "expressions/expression.h"
#include "expressions/terminal/coord_expression.h"
#include "expressions/terminal/number_expression.h"
#include "expressions/terminal/string_expression.h"
#include "expressions/nonterminal/configure_path.h"
#include "expressions/terminal/clear_expression.h"
#include "expressions/terminal/quit_expression.h"
#include "expressions/terminal/view_configs.h"
#include "expressions/nonterminal/add_profile.h"
#include "expressions/terminal/view_profiles_expression.h"
#include "expressions/nonterminal/view_profile_expression.h"
#include "expressions/nonterminal/analyze_expression.h"

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

        AnalyzeExpression* ParseAnalyzeCommand();
        Expression* ParseConfigureCommand();
        Expression* ParseViewConfigCommand();
        ClearExpression* ParseClearCommand();
        QuitExpression* ParseQuitCommand();
        Expression* ParseProfileCommand();

        bool configure_mode_;

        Expression* ParseNext();

    public:
        explicit Parser(const std::vector<Token>& tokens, bool config_mode = false);
        Expression* Parse(); 
};