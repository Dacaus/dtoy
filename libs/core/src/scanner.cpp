#include "scanner.h"

#include <cctype>
#include <stdexcept>
#include <string>

#include "token.h"

namespace dtoy {
namespace scanner {

std::vector<token::Token> Scanner::scan_tokens() {
  while (!is_at_end()) {
    scan_token();
  }
  tokens_.emplace_back(token::TokenType::EOF_, "", line_);
  return tokens_;
}


void Scanner::scan_token() {
  start_ = current_;
  char c = advance();
  if (std::isspace(c)) {
    if (c == '\n') {
      line_++;
    }
    skip_whitespace();
    return;
  }

  // 处理符号
  switch (c) {
  // 单符号
  case '(': add_token(token::TokenType::LEFT_PAREN,sources_.substr(start_, current_ - start_)); break;
  case ')': add_token(token::TokenType::RIGHT_PAREN,sources_.substr(start_, current_ - start_)); break;
  case '{': add_token(token::TokenType::LEFT_BRACE,sources_.substr(start_, current_ - start_)); break;
  case '}': add_token(token::TokenType::RIGHT_BRACE,sources_.substr(start_, current_ - start_)); break;
  case ';': add_token(token::TokenType::SEMICOLON,sources_.substr(start_, current_ - start_)); break;
  case ',': add_token(token::TokenType::COMMA,sources_.substr(start_, current_ - start_)); break;
  case '.': add_token(token::TokenType::DOT,sources_.substr(start_, current_ - start_)); break;
  case '-': add_token(token::TokenType::MINUS,sources_.substr(start_, current_ - start_)); break;
  case '+': add_token(token::TokenType::PLUS,sources_.substr(start_, current_ - start_)); break;
  case '/': add_token(token::TokenType::SLASH,sources_.substr(start_, current_ - start_)); break;
  case '*': add_token(token::TokenType::STAR,sources_.substr(start_, current_ - start_)); break;

  // 双符号
  case '!':
    add_token(match('=') ? token::TokenType::BANG_EQUAL
                         : token::TokenType::BANG,sources_.substr(start_, current_ - start_));
    break;

  case '=':
    add_token(match('=') ? token::TokenType::EQUAL_EQUAL
                         : token::TokenType::EQUAL,sources_.substr(start_, current_ - start_));
    break;

  case '>':
    add_token(match('=') ? token::TokenType::GREATER_EQUAL
                         : token::TokenType::GREATER,sources_.substr(start_, current_ - start_));
    break;

  case '<':
    add_token(match('=') ? token::TokenType::LESS_EQUAL
                         : token::TokenType::LESS,sources_.substr(start_, current_ - start_));
    break;


    // 字符串
  case '"': {
    std::string literal;
    std::string lexeme = "\"";
    while (peek() != '"' && !is_at_end()) {
      if (peek() == '\\') {
        lexeme += advance();  // Consume the backslash
        char escape_char = advance();
        auto [lexeme_part, actual_char] = process_escape_sequence(escape_char);
        lexeme += lexeme_part;
        literal += actual_char;
      } else {
        char temp = advance();
        lexeme += temp;
        literal += temp;
      }
    }

    lexeme += advance();  // consume closing "
    add_token(token::TokenType::STRING, lexeme, token::Literal{literal});
    break;
  }


  case '\'': {
    std::string lexeme = "\'";
    char literal_char;
    if (peek() == '\\') {
      lexeme += advance();  // consume backslash
      char escape_char = advance();
      auto [lexeme_part, actual_char] = process_escape_sequence(escape_char);

      lexeme += lexeme_part;
      literal_char = actual_char;
    } else {
      literal_char = advance();
      lexeme += literal_char;
    }
    if (peek() != '\'') {
      // Handle error: Unterminated char literal
      throw std::runtime_error("Unterminated char literal." +
                               std::string(" at line ") +
                               std::to_string(line_) + " lexeme: " + lexeme);
    }

    lexeme += advance();  // consume closing '
    add_token(token::TokenType::CHAR, lexeme, token::Literal{literal_char});
    break;
  }

  // EOF_
  case '\0': add_token(token::TokenType::EOF_); break;

  default:

    // Handle unexpected characters
    if (std::isalpha(c) || c == '_') {
      identifierAndKeywords();
    } else if (std::isdigit(c)) {
      number();
    }
    break;
  }
}


void Scanner::identifierAndKeywords() {
  // Implementation for scanning identifiers
  while (std::isalnum(peek()) || peek() == '_') {
    advance();
  }

  std::string text = sources_.substr(start_, current_ - start_);
  auto [is_keyword, token_type] = token::Token::isKeyword(text);
  if (is_keyword)
    if (token_type == token::TokenType::TRUE)
      add_token(token_type, true);
    else if (token_type == token::TokenType::FALSE)
      add_token(token_type, false);
    else if(token_type == token::TokenType::NIL)
      add_token(token_type,token::Literal{nullptr});
    else
      add_token(token_type);
  else
    add_token(token::TokenType::IDENTIFIER, text);
}

void Scanner::number() {
  bool is_float = false;
  // Implementation for scanning numbers
  while (std::isdigit(peek())) {
    advance();
  }

  // Look for a fractional part.
  if (peek() == '.' && std::isdigit(peek_next())) {
    is_float = true;
    // Consume the "."
    advance();


    while (std::isdigit(peek())) {
      advance();
    }
  }

  std::string number_str = sources_.substr(start_, current_ - start_);
  if(is_float){
    double value = std::stod(number_str);
    add_token(token::TokenType::NUMBER, number_str, token::Literal{value});
    return;
  }
  else{
    int value = std::stoi(number_str);
    add_token(token::TokenType::NUMBER, number_str, token::Literal{value});
    return;
  }
}


void Scanner::add_token(token::TokenType type) {
  add_token(type, std::string(""));
}

void Scanner::add_token(token::TokenType type, std::string lexeme) {
  add_token(type, lexeme, token::Literal{});
}

void Scanner::add_token(token::TokenType type, token::Literal literal) {
  add_token(type, sources_.substr(start_, current_ - start_), literal);
}

void Scanner::add_token(token::TokenType type, std::string lexeme,
                        token::Literal literal) {
  tokens_.emplace_back(type, lexeme, literal, line_);
}

inline char Scanner::advance() {
  if (is_at_end())
    return '\0';
  return sources_[current_++];
}

const std::vector<token::Token> &Scanner::show_tokens() const {
  return tokens_;
}
bool Scanner::is_at_end() const {
  return current_ >= sources_.length();
}

bool Scanner::match(char expected) {
  if (is_at_end())
    return false;
  if (sources_[current_] != expected)
    return false;

  current_++;
  return true;
}

void Scanner::skip_whitespace() {
  // Implementation for skipping whitespace (if needed)
  char c = advance();

  while (std::isspace(c)) {
    if (c == '\n') {
      line_++;
    }
    if (is_at_end())
      break;
    c = advance();
  }
  current_--;  // Step back one character since we've advanced one too far
}

char Scanner::peek() const {
  if (is_at_end())
    return '\0';
  return sources_[current_];
}

char Scanner::peek_next() const {
  if (current_ + 1 >= sources_.length())
    return '\0';
  return sources_[current_ + 1];
}


std::pair<char, char> Scanner::process_escape_sequence(char escape_char) {
  char lexeme_part;
  char actual_char;
  switch (escape_char) {
  case 'n':
    lexeme_part = escape_char;
    actual_char = '\n';
    break;
  case 't':
    lexeme_part = escape_char;
    actual_char = '\t';
    break;
  case 'r':
    lexeme_part = escape_char;
    actual_char = '\r';
    break;
  case '0':
    lexeme_part = escape_char;
    actual_char = '\0';
    break;
  case '\'':
    lexeme_part = escape_char;
    actual_char = '\'';
    break;
  case '"':
    lexeme_part = escape_char;
    actual_char = '"';
    break;
  case '\\':
    lexeme_part = escape_char;
    actual_char = '\\';
    break;
  default:
    lexeme_part = escape_char;
    lexeme_part += escape_char;
    actual_char = escape_char;
    break;
  }
  return {lexeme_part, actual_char};
}



}  // namespace scanner
}  // namespace dtoy