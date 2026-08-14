#include "interpreter/parser.h"
#include "interpreter/preprocessor.h"

#include "expressions/nonterminal/analyze_manual.h"
#include "expressions/nonterminal/analyze_ticker.h"

#include <iostream>

Parser::Parser(const std::vector<Token>& tokens, bool config_mode) : tokens_(tokens), current_(0), configure_mode_(config_mode) {}

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

Expression* Parser::ParseConfigureCommand() {
    Advance();
    Token mode = Consume(TokenType::IDENTIFIER, "Expected a CONFIGURE mode (e.g. EDIT)");
    
    if (mode.value == "VIEW") {
        return new ViewConfigExpression();
    } else if (mode.value == "EDIT") {
        StringExpression* cik_exp = ParseString();
        StringExpression* model_exp = ParseString();
        StringExpression* profiles_exp = ParseString();

        return new ConfigureExpression(cik_exp, model_exp, profiles_exp);
    }

    throw std::runtime_error("Unknown CONFIGURE mode: " + mode.value);
}

ClearExpression* Parser::ParseClearCommand() {
    Advance();
    return new ClearExpression();
}

QuitExpression* Parser::ParseQuitCommand() {
    Advance();
    return new QuitExpression();
}

Expression* Parser::ParseProfileCommand() {
    Advance();
    Token mode = Consume(TokenType::IDENTIFIER, "Expected mode after PROFILE (e.g., ADD).");

    if (mode.value == "ADD") {
        StringExpression* name = ParseString();
        NumberExpression* nearest_clusters = ParseNumber();
        NumberExpression* nearest_nodes = ParseNumber();
        NumberExpression* clusters_ef = ParseNumber();
        NumberExpression* node_ef = ParseNumber();

        return new AddProfileExpression(name, nearest_clusters, nearest_nodes, clusters_ef, node_ef);
    } else if (mode.value == "VIEW") {
        Token view_mode = Consume(TokenType::IDENTIFIER, "Expected mode after PROFILE VIEW (e.g., ONE).");

        if (view_mode.value == "ALL") {
            return new ViewProfilesExpression();
        } else if (view_mode.value == "ONE") {
            StringExpression* profile = ParseString();
            return new ViewProfileExpression(profile);
        }

        throw std::runtime_error("Unknown PROFILE VIEW mode: " + mode.value);
    }

    throw std::runtime_error("Unknown PROFILE mode: " + mode.value);
}

Expression* Parser::ParseNext() {
    std::string value;

    if (Check(TokenType::IDENTIFIER)) {
        value = Peek().value;

        if (value == "ANALYZE" && !configure_mode_) {
            return ParseAnalyzeCommand();
        } else if (value == "PROFILE" && !configure_mode_) {
            return ParseProfileCommand();
        } else if (value == "CONFIGURE") {
            return ParseConfigureCommand();
        } else if (value == "CLEAR") {
            return ParseClearCommand();
        } else if (value == "QUIT") {
            return ParseQuitCommand();
        }
    }

    std::string error_string = "Unrecognized command " + value + ".";
    if (configure_mode_) {
        error_string += "\nApp is still in configure mode, can only run CONFIGURE, CLEAR, and QUIT";
    }

    throw std::runtime_error(error_string);
}

Expression* Parser::Parse() {
    if (IsAtEnd()) return nullptr;

    Expression* ast = nullptr;
    Expression* current_exp = nullptr;

    while (!IsAtEnd()) {
        Expression* next_exp = ParseNext();

        if (ast == nullptr && current_exp == nullptr) {
            ast = next_exp;
            current_exp = next_exp;
        } else {
            current_exp -> SetNextExpression(next_exp);
            current_exp = current_exp -> GetNextExpression();
        }
    }
    
    return ast;
}