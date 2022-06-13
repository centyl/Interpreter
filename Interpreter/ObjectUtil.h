#pragma once

#include <utility>

#include "Object.h"
#include "Operation.h"

namespace Util {
    namespace Obj {

        namespace {
            int rem(int a, int b) {
                return a % b;
            }

            double rem(double a, double b) {
                return fmod(a, b);
            }
        }

        template <typename T>
        ObjectOrError result(Operation operation, T a, T b) {
            switch (operation) {
                case Operation::Add:
                    return Objects::getOrCreate(a + b);
                case Operation::Subtract:
                    return Objects::getOrCreate(a - b);
                case Operation::Multiply:
                    return Objects::getOrCreate(a * b);
                case Operation::Mod:
                    return Objects::getOrCreate(rem(a, b));
                case Operation::Greater:
                    return Objects::get(a > b);
                case Operation::GreaterEqual:
                    return Objects::get(a >= b);
                case Operation::Less:
                    return Objects::get(a < b);
                case Operation::LessEqual:
                    return Objects::get(a <= b);
                case Operation::Square:
                    return Objects::getOrCreate((T) pow(a, b));
                case Operation::Equal:
                    return Objects::get(a == b);
                case Operation::NotEqual:
                    return Objects::get(a != b);
                case Operation::Divide:
                    return Objects::getOrCreate(a / (double) b);
                case Operation::FloorDivision:
                    return Objects::getOrCreate((int) (a / b));
            }
            return std::make_unique<Error>(ErrorType::UnknownError);
        }

    }
}