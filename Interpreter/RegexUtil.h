#pragma once

#include <regex>

namespace Util {
    namespace Regex {
        bool isDouble(const std::string_view sv);

        std::regex_iterator<std::string_view::const_iterator> operandIterator(const std::string_view sv);

        bool isFunctionDefinition(const std::string_view sv);

        bool isAugmentedAssignment(const std::string_view sv);
        bool isMethodDefinition(const std::string_view sv);
    }
}