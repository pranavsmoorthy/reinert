#include "interpreter/parser.h"

Parser::Parser(const std::vector<Token>& tokens) : tokens_(tokens), current_(0) {}

Token Parser::Peek() const {
    return tokens_[current_];
}

bool Parser::IsAtEnd() const {
    return Peek().type == TokenType::END_OF_FILE;
}

Token Parser::Advance() {
    if (!IsAtEnd()) current_++;
    return tokens_[current_ - 1];
}

bool Parser::Check(TokenType type) const {
    if (IsAtEnd()) return false;
    return Peek().type == type;
}

Token Parser::Consume(TokenType type, const std::string& message) {
    if (Check(type)) return Advance();
    throw std::runtime_error("Syntax Error: " + message);
}

CoordExpression* Parser::ParseVector() {
    Consume(TokenType::L_BRACKET, "Expected '[' to start vector.");

    std::array<double, 12> coords;
    for (size_t i = 0; i < 12; ++i) {
        Token numToken = Consume(TokenType::NUMBER, "Expected a number in vector.");
        coords[i] = std::stod(numToken.value);

        if (i < 11) {
            Consume(TokenType::COMMA, "Coordinates must have 12 elements. Make sure a ',' is between vector coordinates.");
        }
    }

    Consume(TokenType::R_BRACKET, "Expected ']' to end vector.");
    
    return new CoordExpression(coords);
}

Expression* Parser::ParseAnalyzeCommand() {
    Advance(); 

    Token mode = Consume(TokenType::IDENTIFIER, "Expected mode after ANALYZE (e.g., MANUAL).");

    if (mode.value == "MANUAL") {
        CoordExpression* coords = ParseVector();
        return new AnalyzeManualExpression(coords); 
    }

    throw std::runtime_error("Unknown ANALYZE mode: " + mode.value);
}

Expression* Parser::Parse() {
    if (IsAtEnd()) return nullptr;

    if (Check(TokenType::IDENTIFIER) && Peek().value == "ANALYZE") {
        return ParseAnalyzeCommand();
    }

    throw std::runtime_error("Unrecognized command.");
}