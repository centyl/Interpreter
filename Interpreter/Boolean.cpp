#include "Boolean.h"
#include "Objects.h"

using namespace std;

ObjectOrError Boolean::execute(Operation operation, Object* other) {
    return Objects::getOrCreate(value)->execute(operation, other);
}

Boolean::Boolean(bool b) : value(b) { }

optional<string> Boolean::repr() {
    return (value ? "true" : "false");
}

optional<int> Boolean::intValue() {
    return value;
}

optional<bool> Boolean::booleanValue() {
    return value;
}