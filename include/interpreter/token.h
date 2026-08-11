#pragma once

#ifndef INCLUDE_INTERPRETER_TOKEN_H_
#define INCLUDE_INTERPRETER_TOKEN_H_

#include <string>

enum class TokenType { 
    IDENTIFIER, 
    L_BRACKET, 
    R_BRACKET, 
    NUMBER, 
    COMMA, 
    END_OF_FILE,
};

struct Token {
    TokenType type;
    std::string value;
};

#endif