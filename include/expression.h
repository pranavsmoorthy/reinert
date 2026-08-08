#ifndef INCLUDE_EXPRESSION_H_
#define INCLUDE_EXPRESSION_H_

#include "context.h"

#include <string>
#include <stdexcept>
#include <array>

template<typename T, std::size_t L>
class Expression {
    protected:
        std::array<Expression*, L> leaves_{};

    public:
        virtual ~Expression() = default;

        virtual T Execute(const Context& ctx) const = 0;

        void ThrowCannotExecuteCommand(const std::string& additional_info) const {
            throw std::logic_error("Command could not execute: " + additional_info);
        }

        void ThrowCannotExecuteCommand() const {
            throw std::logic_error("Command could not execute");
        }
};

#endif