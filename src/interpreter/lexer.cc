#include "interpreter/lexer.h"
#include <stdexcept>

Lexer::Lexer(const std::string& source) : source_(source) {}

bool Lexer::IsAtEnd() const {
    return current_ >= source_.length();
}

char Lexer::Advance() {
    return source_[current_++];
}

char Lexer::Peek() const {
    if (IsAtEnd()) return '\0';
    return source_[current_];
}

char Lexer::PeekNext() const {
    if (current_ + 1 >= source_.length()) return '\0';
    return source_[current_ + 1];
}

void Lexer::AddToken(TokenType type) {
    std::string text = source_.substr(start_, current_ - start_);
    tokens_.push_back({type, text});
}

bool Lexer::IsDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::IsAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || 
           (c >= 'A' && c <= 'Z') || 
            c == '_' || c == '/' || c == '.';
}

std::vector<Token> Lexer::Tokenize() {
    while (!IsAtEnd()) {
        start_ = current_;
        ScanToken();
    }
    
    tokens_.push_back({TokenType::END_OF_FILE, ""});
    return tokens_;
}

void Lexer::ScanToken() {
    char c = Advance();
    switch (c) {
        case '[': AddToken(TokenType::L_BRACKET); break;
        case ']': AddToken(TokenType::R_BRACKET); break;
        case ',': AddToken(TokenType::COMMA); break;
        
        // Ignore whitespace
        case ' ': 
        case '\r': 
        case '\t': 
        case '\n': 
            break;
            
        // Negative numbers
        case '-':
            if (IsDigit(Peek())) {
                Number();
            } else {
                throw std::runtime_error("Unexpected character: -");
            }
            break;

        default:
            if (IsDigit(c)) {
                Number();
            } else if (IsAlpha(c)) {
                Identifier();
            } else {
                throw std::runtime_error(std::string("Unexpected character: ") + c);
            }
            break;
    }
}

void Lexer::Number() {
    while (IsDigit(Peek())) Advance();

    if (Peek() == '.' && IsDigit(PeekNext())) {
        Advance();
        while (IsDigit(Peek())) Advance();
    }

    AddToken(TokenType::NUMBER);
}

void Lexer::Identifier() {
    while (IsAlpha(Peek()) || IsDigit(Peek())) {
        Advance();
    }

    AddToken(TokenType::IDENTIFIER);
}