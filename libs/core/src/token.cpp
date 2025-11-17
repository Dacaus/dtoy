#include "token.h"

#include <format>
#include <string>
namespace dtoy {
namespace token {
std::map<std::string, TokenType> Token::keywords = {
    {"and", TokenType::AND},       {"class", TokenType::CLASS},
    {"else", TokenType::ELSE},     {"false", TokenType::FALSE},
    {"fun", TokenType::FUN},       {"for", TokenType::FOR},
    {"if", TokenType::IF},         {"nil", TokenType::NIL},
    {"or", TokenType::OR},         {"print", TokenType::PRINT},
    {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
    {"this", TokenType::THIS},     {"true", TokenType::TRUE},
    {"var", TokenType::VAR},       {"while", TokenType::WHILE},
};
std::map<TokenType, std::string> Token::type_names = {
    // Single-character tokens.
    {TokenType::LEFT_PAREN, "LEFT_PAREN"},
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenType::LEFT_BRACE, "LEFT_BRACE"},
    {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::DOT, "DOT"},
    {TokenType::MINUS, "MINUS"},
    {TokenType::PLUS, "PLUS"},
    {TokenType::SEMICOLON, "SEMICOLON"},
    {TokenType::SLASH, "SLASH"},
    {TokenType::STAR, "STAR"},

    // One or two character tokens.
    {TokenType::BANG, "BANG"},
    {TokenType::BANG_EQUAL, "BANG_EQUAL"},
    {TokenType::EQUAL, "EQUAL"},
    {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
    {TokenType::GREATER, "GREATER"},
    {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
    {TokenType::LESS, "LESS"},
    {TokenType::LESS_EQUAL, "LESS_EQUAL"},

    // Literals.
    {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::STRING, "STRING"},
    {TokenType::CHAR, "CHAR"},
    {TokenType::NUMBER, "NUMBER"},
    {TokenType::BOOL, "BOOL"},
    // Keywords.
    {TokenType::AND, "AND"},
    {TokenType::CLASS, "CLASS"},
    {TokenType::ELSE, "ELSE"},
    {TokenType::FALSE, "FALSE"},
    {TokenType::FUN, "FUN"},
    {TokenType::FOR, "FOR"},
    {TokenType::IF, "IF"},
    {TokenType::NIL, "NIL"},
    {TokenType::OR, "OR"},
    {TokenType::PRINT, "PRINT"},

    {TokenType::RETURN, "RETURN"},
    {TokenType::SUPER, "SUPER"},
    {TokenType::THIS, "THIS"},
    {TokenType::TRUE, "TRUE"},
    {TokenType::VAR, "VAR"},
    {TokenType::WHILE, "WHILE"},
    {TokenType::EOF_, "EOF_"},

};

std::string Token::to_string() const {
  return std::format("Token:{{ type: {}, lexeme: '{}', literal:{} }} ",
                     type_name(), lexeme(), literalToString());
};

std::string Token::literalToString() const {
  struct literalVistor {
    std::string operator()(const std::monostate &) const {
      return "nil";
    }
    std::string operator()(const std::string &s) const {
      return s;
    }
    std::string operator()(const char &c) const {
      return std::string(1, c);
    }
    std::string operator()(const int &i) const {
      return std::to_string(i);
    }
    std::string operator()(const double &d) const {
      return std::to_string(d);
    }
  };
  return std::visit(literalVistor{}, literal_);
};

std::pair<bool, TokenType> Token::isKeyword(const std::string &str) {
  auto it = keywords.find(str);
  if (it != keywords.end()) {
    return {true, it->second};
  } else {
    return {false, TokenType::IDENTIFIER};
  }
}

}  // namespace token
}  // namespace dtoy