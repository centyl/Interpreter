#include <optional>
#include <memory>
#include <vector>

#include "Reference.h"
#include "ReferenceStack.h"

using namespace std;

string_view Reference::getName() {
    return name;
}

Reference::Reference(const string_view name) : name(name) {

}

// find correct variable in current reference frame, return its integer interpretation
optional<int> Reference::intValue() {
    auto object = ReferenceStack::get(name);
    return (object ? object->intValue() : std::nullopt);
}

optional<double> Reference::doubleValue() {
    auto object = ReferenceStack::get(name);
    return (object ? object->doubleValue() : std::nullopt);
}

// save value under correct variable name in current reference frame
ObjectOrError Reference::assign(Object* other) {
    auto oldValue = ReferenceStack::get(name);
    if (oldValue != other) {
        if (auto unrefd = other->unref(); isObject(unrefd)) {
            if (oldValue) {
                oldValue->decrementAndDestroy();
            }
            ReferenceStack::set(name, get<Object*>(unrefd));
        }
    }
    return other;
}

// execute operation using underlying object, unless it's an assignment op
ObjectOrError Reference::execute(Operation operation, Object* other) {
    if (operation == Operation::Assign) return assign(other);
    auto object = ReferenceStack::get(name);
    return (object ? object->execute(operation, other) : make_unique<Error>(ErrorType::UnknownError));
}

optional<string> Reference::repr() {
    auto object = ReferenceStack::get(name);
    return (object ? object->repr() : nullopt);
}

ObjectOrError Reference::runMethod(const string_view s, const vector<Object*>& arguments) {
    auto object = ReferenceStack::get(name);
    return (object
            ? object->runMethod(s, arguments)
            : make_unique<Error>(ErrorType::VariableNotFound));
}

ObjectOrError Reference::runMethod(const string_view s, const string_view argumentName, Node* n) {
    auto object = ReferenceStack::get(name);
    return (object ? object->runMethod(s, argumentName, n) : make_unique<Error>(ErrorType::UnknownError));
}

ObjectOrError Reference::unref() {
    auto object = ReferenceStack::get(name);
    return (object ? object : (ObjectOrError) make_unique<Error>(ErrorType::UnknownError));
}

optional<bool> Reference::booleanValue() {
    auto object = ReferenceStack::get(name);
    return (object ? object->booleanValue() : nullopt);
}