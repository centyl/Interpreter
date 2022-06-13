#include <map>
#include <string>

#include "Error.h"

using namespace std;

namespace {
    std::map<ErrorType, std::string> errorMessages {
        { ErrorType::InvalidArgumentCount, "Invalid argument count" },
        { ErrorType::InvalidLambdaBody, "Invalid lambda body" },
        { ErrorType::InvalidLambdaArgument, "Invalid lambda argument" },
        { ErrorType::UnknownError, "UnknownError" },
        { ErrorType::InvalidOperandCount, "Invalid operand count" },
        { ErrorType::NotImplemented, "Not implemented" },
        { ErrorType::NoBooleanValue, "Operand cannot be interpreted as Boolean" },
        { ErrorType::ConstructorNotFound, "Constructor not found" },
        { ErrorType::ConversionToIntFailed, "Cannot convert object to Integer" },
        { ErrorType::VariableNotFound, "Variable not found" }
    };
}

Error::Error(ErrorType type) : type(type) {

}

string_view Error::getErrorMessage() {
    return errorMessages[type];
}
