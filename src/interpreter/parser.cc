#include "interpreter/parser.h"
#include "interpreter/preprocessor.h"

#include "expressions/nonterminal/analyze_manual.h"
#include "expressions/nonterminal/analyze_ticker.h"

#include <iostream>

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
    
    std::array<double, 12> coords_transformed = Preprocessor::TransformCoordinates(coords);

    for (double d : coords_transformed) {
        std::cout << d << " ";
    }

    std::cout << std::endl;

    return new CoordExpression(coords_transformed);
}

NumberExpression* Parser::ParseNumber() {
    Token token = Consume(TokenType::NUMBER, "Expected a number.");

    try {
        double value = std::stod(token.value);
        
        return new NumberExpression(value);
        
    } catch (const std::invalid_argument& e) {
        throw std::runtime_error("Parser Error: Invalid number format encountered '" + token.value + "'");
    } catch (const std::out_of_range& e) {
        throw std::runtime_error("Parser Error: Number out of range '" + token.value + "'");
    }
}

StringExpression* Parser::ParseString() {
    Token token = Consume(TokenType::IDENTIFIER, "Expected a string literal.");
    std::string value = token.value;

    return new StringExpression(value);
}

Expression* Parser::ParseAnalyzeCommand() {
    Advance(); 

    Token mode = Consume(TokenType::IDENTIFIER, "Expected mode after ANALYZE (e.g., MANUAL).");

    if (mode.value == "MANUAL") {
        CoordExpression* coords = ParseVector();
        return new AnalyzeManualExpression(coords); 
    } else if (mode.value == "TICKER") {
        StringExpression* ticker = ParseString();
        NumberExpression* consecutive_eps_beats = ParseNumber();
        NumberExpression* avg_eps_surprise = ParseNumber();
        return new AnalyzeTickerExpression(ticker, consecutive_eps_beats, avg_eps_surprise);
    }

    throw std::runtime_error("Unknown ANALYZE mode: " + mode.value);
}

ClearExpression* Parser::ParseClearCommand() {
    return new ClearExpression();
}

QuitExpression* Parser::ParseQuitCommand() {
    return new QuitExpression();
}

Expression* Parser::Parse() {
    if (IsAtEnd()) return nullptr;

    if (Check(TokenType::IDENTIFIER)) {
        std::string value = Peek().value;

        if (value == "ANALYZE") {
            return ParseAnalyzeCommand();
        } else if (value == "CLEAR") {
            return ParseClearCommand();
        } else if (value == "QUIT") {
            return ParseQuitCommand();
        }
    }

    throw std::runtime_error("Unrecognized command.");
}