#pragma once

#ifndef INCLUDE_EXPRESSION_H_
#define INCLUDE_EXPRESSION_H_

#include "context.h"

#include <string>
#include <stdexcept>
#include <any>

class Expression {
    private:
        Expression* next_ = nullptr;

    public:
        virtual ~Expression() {
            delete next_;
            next_ = nullptr;
        }

        void Run(Context& ctx) {
            Execute(ctx);

            if (next_ != nullptr) {
                next_ -> Run(ctx);
            }
        }

        virtual std::any Execute(Context& ctx) const = 0;
        
        Expression* GetNextExpression() const {
            return next_;
        }

        void SetNextExpression(Expression* exp) {
            next_ = exp;
        }

        void ThrowCannotExecuteCommand(const std::string& additional_info) const {
            throw std::runtime_error("Command could not execute: " + additional_info);
        }

        void ThrowCannotExecuteCommand() const {
            throw std::runtime_error("Command could not execute");
        }
};

#endif