#pragma once

#ifndef INCLUDE_EXPRESSIONS_NONTERMINAL_ANALYZE_EXPRESSION_H_
#define INCLUDE_EXPRESSIONS_NONTERMINAL_ANALYZE_EXPRESSION_H_

#include "expressions/expression.h"
#include "expressions/terminal/string_expression.h"

/**
 * @class AnalyzeExpression
 * @brief Base class for expressions that perform analysis operations.
 * 
 * This class provides common state and functionality for all analysis-related commands 
 * (such as analyzing by ticker or manual coordinates). Specifically, it manages an optional 
 * search profile that dictates the parameters of the vector database query.
 */
class AnalyzeExpression : public Expression {
    private:
        /**
         * @brief Pointer to the AST node containing the parsed profile name.
         * 
         * If this remains nullptr, the execution context should fall back to using 
         * the default search profile.
         */
        StringExpression* profile_expression_ = nullptr;

    public:
        /**
         * @brief Destructor responsible for safely cleaning up the profile expression.
         * 
         * Automatically deletes the dynamically allocated profile expression if one was 
         * attached, preventing memory leaks during AST destruction.
         */
        ~AnalyzeExpression() {
            if (profile_expression_ != nullptr) {
                delete profile_expression_;
                profile_expression_ = nullptr;   
            }
        }

        /**
         * @brief Attaches a custom search profile identifier to this analysis command.
         * 
         * @param profile_name Pointer to the StringExpression holding the profile's name.
         */
        void SetProfile(StringExpression* profile_name) {
            profile_expression_ = profile_name;
        }

        /**
         * @brief Retrieves the assigned search profile expression.
         * 
         * @return const StringExpression* A read-only pointer to the profile expression, 
         *         or nullptr if no specific profile was assigned.
         */
        const StringExpression* GetProfile() const {
            return profile_expression_;
        }
};

#endif