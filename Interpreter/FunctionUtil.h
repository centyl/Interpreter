#pragma once

#include "Node.h"

namespace Util {

    namespace Function {

        int getArgumentCount(std::vector<Operand>::const_iterator begin, std::vector<Operand>::const_iterator end);

        void pushArguments(const std::vector<std::string>& argumentNames, const std::vector<Object*>& arguments);

        ObjectsOrError getArguments(std::vector<Operand>::const_iterator begin, std::vector<Operand>::const_iterator end);

    }
}