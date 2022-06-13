#pragma once

#include <variant>
#include <vector>
#include <optional>
#include <memory>
#include <string>

#include "Operation.h"
#include "Error.h"

template <typename T>
using OptionalError = std::variant<T, std::unique_ptr<Error>>;

template <typename T>
auto isObject(OptionalError<T>& x) {
    return std::holds_alternative<T>(x);
}
template <typename T>
auto isError(OptionalError<T>& x) {
    return std::holds_alternative<std::unique_ptr<Error>>(x);
}

class Object;

using ObjectOrError = OptionalError<Object*>;
using ObjectsOrError = OptionalError<std::vector<Object*>>;

class Node;

class Object {
    protected:
    int referenceCounter = 0;
    public:

    // string representation of object
    virtual std::optional<std::string> repr() {
        return std::nullopt;
    }

    // execute binary operation with lhs=this, and rhs=other object
    virtual ObjectOrError execute(Operation operation, Object* other) {
        return std::move(std::make_unique<Error>(ErrorType::UnknownError));
    }

    // int representation of object
    virtual std::optional<int> intValue() { return std::nullopt; }

    // bool representation of object
    virtual std::optional<bool> booleanValue() { return std::nullopt; }

    // double representation of object
    virtual std::optional<double> doubleValue() { return std::nullopt; }

    virtual void incrementReferenceCounter() final;

    virtual void decrementReferenceCounter() final;

    virtual void erase();

    virtual void destructIfNotReferenced() final;

    virtual void decrementAndDestroy() final;

    // run a method with 0 arguments 
    ObjectOrError runMethod(const std::string_view s) {
        return runMethod(s, std::vector<Object*>{});
    }

    // run a method with a vector of arguments
    virtual ObjectOrError runMethod(const std::string_view s, const std::vector<Object*>& arguments) {
        return std::make_unique<Error>(ErrorType::UnknownError);
    }

    // run a method with 1 argument
    ObjectOrError runMethod(const std::string_view s, Object* o) {
        return runMethod(s, std::vector<Object*>{o});
    }

    // run a method with lambda expression as argument
    virtual ObjectOrError runMethod(const std::string_view s, const std::string_view argumentName, Node* n) {
        return std::make_unique<Error>(ErrorType::UnknownError);
    }

    // this method is overriden for Reference type, to return the object it's pointing at
    virtual ObjectOrError unref() {
        return this;
    }

};