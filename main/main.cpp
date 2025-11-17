#include <iostream>
#include <string>
#include <vector>

#include "interpreter.h"
#include "parser.h"
#include "scanner.h"
#include "stmt.h"

using namespace dtoy;

void run(const std::string &source) {
  try {
    // 词法分析
    scanner::Scanner scanner(source);
    std::vector<token::Token> tokens = scanner.scan_tokens();

    // // 语法分析
    // parser::Parser parser(tokens);
    // std::unique_ptr<expr::Expr> expression = parser.expression();

    parser::Parser parser(tokens);
    std::vector<std::unique_ptr<stmt::Stmt>> statements = parser.parse();
    interpreter::Interpreter interpreter;
    interpreter.interpret(statements);


    
    // // 解释执行
    // interpreter::Interpreter interpreter;
    // interpreter.interpret(*expression);

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

void runFile(const std::string &filename) {
  // 这里可以添加从文件读取代码的功能
  std::cout << "File execution not implemented yet." << std::endl;
}

void runPrompt() {
  std::string line;

  std::cout << "dtoy Interactive Interpreter" << std::endl;
  std::cout << "Enter expressions to evaluate. Type 'exit' to quit."
            << std::endl;

  while (true) {
    std::cout << "> ";

    if (!std::getline(std::cin, line)) {
      break; // EOF
    }

    if (line == "exit" || line == "quit") {
      break;
    }

    if (!line.empty()) {
      run(line);
    }
  }

  std::cout << "Goodbye!" << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc > 2) {
    std::cout << "Usage: dtoy [script]"  << std::endl;
    return 1;
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    runPrompt();
  }
  return 0;
}