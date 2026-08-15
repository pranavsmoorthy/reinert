#pragma once

#include "token.h"
#include <string>
#include <vector>

/**
 * @class Lexer
 * @brief A lexical analyzer that converts a raw input string into a sequence of tokens.
 * 
 * The Lexer scans the input character by character, grouping them into meaningful 
 * lexemes and generating corresponding Token objects for the Parser to evaluate.
 */
class Lexer {
private:
    std::string source_;        ///< The raw input string being scanned.
    std::vector<Token> tokens_; ///< The collection of tokens generated from the source.
    
    size_t start_ = 0;          ///< The starting index of the current lexeme being scanned.
    size_t current_ = 0;        ///< The index of the character currently being evaluated.

    /**
     * @brief Checks if the scanner has reached the end of the source string.
     * @return true if the current index is at or beyond the source length, false otherwise.
     */
    bool IsAtEnd() const;

    /**
     * @brief Consumes the current character and advances the scanner to the next.
     * @return The character that was just consumed.
     */
    char Advance();

    /**
     * @brief Looks at the current character without consuming it.
     * @return The current character, or '\0' if at the end.
     */
    char Peek() const;

    /**
     * @brief Looks ahead at the next character without consuming it.
     * @return The next character, or '\0' if at the end.
     */
    char PeekNext() const;
    
    /**
     * @brief Creates a token of the specified type and appends it to the token list.
     * @param type The TokenType to assign to the newly created token.
     */
    void AddToken(TokenType type);

    /**
     * @brief Evaluates the current character and branches to the appropriate tokenization logic.
     */
    void ScanToken();

    /**
     * @brief Consumes a sequence of digits to generate a numeric token.
     */
    void Number();

    /**
     * @brief Consumes an alphanumeric sequence to generate an IDENTIFIER token.
     */
    void Identifier();

    /**
     * @brief Checks if a character is a numeric digit.
     * @param c The character to evaluate.
     * @return true if the character is between '0' and '9', false otherwise.
     */
    bool IsDigit(char c) const;

    /**
     * @brief Checks if a character is an alphabetical letter or an underscore.
     * @param c The character to evaluate.
     * @return true if the character is 'a'-'z', 'A'-'Z', or '_', false otherwise.
     */
    bool IsAlpha(char c) const;

public:
    /**
     * @brief Constructs a new Lexer with the provided source string.
     * @param source The raw string to be tokenized.
     */
    explicit Lexer(const std::string& source);

    /**
     * @brief Executes the lexical analysis process.
     * 
     * Iterates through the entire source string, scanning and classifying tokens 
     * until the end of the file is reached.
     * 
     * @return std::vector<Token> The complete sequence of generated tokens.
     */
    std::vector<Token> Tokenize();
};