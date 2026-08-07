#ifndef INCLUDE_EXPRESSION_H_
#define INCLUDE_EXPRESSION_H_

#include "context.h"

#include <string>
#include <stdexcept>

class Expression {
    public:
        virtual ~Expression() = default;

        void ThrowCannotExecuteCommand(const std::string& additional_info) {
            throw std::logic_error("Command could not execute: " + additional_info);
        }

        void ThrowCannotExecuteCommand() {
            throw std::logic_error("Command could not execute");
        }
};

#endif