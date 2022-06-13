#include "RegexUtil.h"

#include <string_view>

namespace Util {
    namespace Regex {
        using namespace std;

        namespace {
            auto regex_double = regex { R"(\d*\.\d+)" };

            auto regex_operand = regex { R"(([\-=]?>)|(([\+\-<=]|\*{1,2}|/{1,2})=?)|[\.(){},%]|!=|\|{2}|&{2}|')" };

            auto regex_function = regex { R"(^\s*function\s+[a-z]+)" };

            auto regex_method = regex { R"(^\s*function\s+[A-Z][a-z]*\.[a-z]+)" };
            auto regex_method_partial = regex { R"(^\s*function\s+[A-Z][a-z]*)" };

            auto regex_constructor = regex { R"(^\s*constructor\s+[A-Z][a-z]*)" };

            auto regex_augmented_assignment = regex { R"((..|[^=!<])=)" };
        }

        bool isDouble(const std::string_view sv) {
            return regex_match(cbegin(sv), cend(sv), regex_double);
        }

        regex_iterator<string_view::const_iterator> operandIterator(const std::string_view sv) {
            return regex_iterator<string_view::const_iterator>(cbegin(sv), cend(sv), regex_operand);
        }

        bool isFunctionDefinition(const string_view sv) {
            return regex_search(cbegin(sv), cend(sv), regex_function)
                || regex_search(cbegin(sv), cend(sv), regex_method)
                || regex_search(cbegin(sv), cend(sv), regex_constructor);
        }

        bool isAugmentedAssignment(const string_view sv) {
            return regex_match(cbegin(sv), cend(sv), regex_augmented_assignment);
        }

        bool isMethodDefinition(const string_view sv) {
            return regex_match(cbegin(sv), cend(sv), regex_method_partial);
        }
    }
}