#include <string>

#include "Double.h"
#include "Objects.h"
#include "ObjectUtil.h"

using namespace std;

ObjectOrError Double::execute(Operation operation, Object* other) {
    auto otherVal = other->doubleValue();

    return (otherVal.has_value()
            ? Util::Obj::result(operation, this->value, otherVal.value())
            : make_unique<Error>(ErrorType::UnknownError));
}

optional<string> Double::repr() {
    return to_string(value);
}

optional<int> Double::intValue() {
    return value;
}

optional<double> Double::doubleValue() {
    return value;
}

void Double::erase() {
    Objects::remove(value);
}
