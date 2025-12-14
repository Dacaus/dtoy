
#include "token.h"
#include <map>
#include <stdexcept>
#include <string>
namespace dtoy {
namespace enviroment {
class Enviroment {
public:
    using Literal = token::Literal;
    std::map<std::string, token::Literal> values_;
    Enviroment() = default;
    ~Enviroment() = default;
    void define(const std::string& name, const Literal& value) {
        values_[name] = value;
    }
    Literal get(const std::string& name) {
        if (values_.find(name) != values_.end()) {
            return values_[name];
        }
        throw std::runtime_error("Undefined variable '" + name + "'.");
    }

    Literal assign(const std::string& name, const Literal& value) {
        if (values_.find(name) != values_.end()) {
            values_[name] = value;
            return value;
        }
        throw std::runtime_error("Undefined variable '" + name + "'.");
    }
};

} // namespace enviroment
} // namespace dtoy