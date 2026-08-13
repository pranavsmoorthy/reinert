#pragma once

#ifndef INCLUDE_EXPRESSION_H_
#define INCLUDE_EXPRESSION_H_

#include "context.h"

#include <string>
#include <stdexcept>
#include <any>

class Expression {
    public:
        virtual ~Expression() = default;
        virtual std::any Execute(const Context& ctx) const = 0;

        void ThrowCannotExecuteCommand(const std::string& additional_info) const {
            throw std::logic_error("Command could not execute: " + additional_info);
        }

        void ThrowCannotExecuteCommand() const {
            throw std::logic_error("Command could not execute");
        }
};

#endif