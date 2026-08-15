#pragma once

#ifndef INCLUDE_EXPRESSION_H_
#define INCLUDE_EXPRESSION_H_

#include "context.h"

#include <string>
#include <stdexcept>
#include <any>

/**
 * @class Expression
 * @brief Abstract base class for all Abstract Syntax Tree (AST) nodes.
 * 
 * This class defines the standard interface for executable commands within the application.
 * It utilizes a chained execution model (similar to a linked list), allowing multiple 
 * expressions to be linked and executed sequentially.
 */
class Expression {
    private:
        /**
         * @brief Pointer to the next expression in the execution chain.
         */
        Expression* next_ = nullptr;

    public:
        /**
         * @brief Virtual destructor.
         * 
         * Safely deletes the next expression in the chain, causing a cascading 
         * deletion of all subsequent linked expressions to prevent memory leaks.
         */
        virtual ~Expression() {
            delete next_;
            next_ = nullptr;
        }

        /**
         * @brief Executes this expression and all subsequent expressions in the chain.
         * 
         * @param ctx The global memory and state context of the application.
         */
        void Run(Context& ctx) {
            Execute(ctx);

            if (next_ != nullptr) {
                next_ -> Run(ctx);
            }
        }

        /**
         * @brief Pure virtual method containing the specific execution logic for the AST node.
         * 
         * @param ctx The global memory and state context of the application.
         * @return std::any An optional return value from the command execution.
         */
        virtual std::any Execute(Context& ctx) const = 0;
        
        /**
         * @brief Retrieves the next expression in the execution chain.
         * 
         * @return Expression* Pointer to the next expression.
         */
        Expression* GetNextExpression() const {
            return next_;
        }

        /**
         * @brief Sets the next expression to be executed after this one.
         * 
         * @param exp Pointer to the expression to append to the chain.
         */
        void SetNextExpression(Expression* exp) {
            next_ = exp;
        }

        /**
         * @brief Throws a standard runtime error indicating execution failure, with additional details.
         * 
         * @param additional_info Specific details regarding why the execution failed.
         * @throws std::runtime_error Always thrown when called.
         */
        void ThrowCannotExecuteCommand(const std::string& additional_info) const {
            throw std::runtime_error("Command could not execute: " + additional_info);
        }

        /**
         * @brief Throws a standard runtime error indicating execution failure.
         * 
         * @throws std::runtime_error Always thrown when called.
         */
        void ThrowCannotExecuteCommand() const {
            throw std::runtime_error("Command could not execute");
        }
};

#endif