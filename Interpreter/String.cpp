#include <cctype>
#include <algorithm>
#include <iterator>

#include "String.h"
#include "Objects.h"

using namespace std;

String::String(const std::string_view value) {
    this->value = value;
}

void String::erase() {
    delete this;
}

ObjectOrError String::execute(Operation operation, Object* other) {
    if (operation == Operation::Add) { // 'abc' + 2 = 'abc2'
        return new String(value + other->repr().value());
    }
    else if (operation == Operation::Multiply) { // 'abc' * 3 = 'abcabcabc'
        if (auto v = other->intValue(); v.has_value()) {
            auto res = string();
            for (int i = 0; i < v.value(); i++) {
                res.append(value);
            }
            return new String(res);
        }
    }
    else if (operation == Operation::Equal) { // '123' == 123 = true
        if (auto s = other->repr(); s.has_value()) {
            return Objects::get(value == s.value());
        }
        return make_unique<Error>(ErrorType::UnknownError);
    }
    return make_unique<Error>(ErrorType::UnknownError);
}

std::optional<std::string> String::repr() {
    return value;
}

ObjectOrError String::runMethod(const std::string_view s, const std::vector<Object*>& arguments) {
    if (s == "toupper") { // 'abc'.toupper = 'ABC'
        if (arguments.empty()) {
            auto res = string();
            transform(cbegin(value), cend(value), back_inserter(res),
                      [](char c) -> char {return toupper(c); });
            return new String(res);
        }
    }
    if (s == "length") {
        if (arguments.empty()) {
            return Objects::getOrCreate((int) value.length());
        }
    }

    return make_unique<Error>(ErrorType::UnknownError);
}
