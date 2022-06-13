#pragma once

enum class ErrorType {
    InvalidArgumentCount,
    InvalidLambdaArgument,
    InvalidLambdaBody,
    UnknownError,
    InvalidOperandCount,
    NotImplemented,
    NoBooleanValue,
    ConstructorNotFound,
    ConversionToIntFailed,
    VariableNotFound
};

class Error {
    ErrorType type;
    public:
    Error(ErrorType type);

    std::string_view getErrorMessage();
};