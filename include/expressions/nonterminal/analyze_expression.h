#pragma once

#ifndef INCLUDE_EXPRESSIONS_NONTERMINAL_ANALYZE_EXPRESSION_H_
#define INCLUDE_EXPRESSIONS_NONTERMINAL_ANALYZE_EXPRESSION_H_

#include "expressions/expression.h"
#include "expressions/terminal/string_expression.h"

class AnalyzeExpression : public Expression {
    private:
        StringExpression* profile_expression_ = nullptr;

    public:
        ~AnalyzeExpression() {
            if (profile_expression_ != nullptr) {
                delete profile_expression_;
                profile_expression_ = nullptr;   
            }
        }

        void SetProfile(StringExpression* profile_name) {
            profile_expression_ = profile_name;
        }

        const StringExpression* GetProfile() const {
            return profile_expression_;
        }
};

#endif