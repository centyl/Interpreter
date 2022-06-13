#include <string>

#include "Integer.h"
#include "Objects.h"
#include "ObjectUtil.h"

using namespace std;

Integer::Integer(int v) : value(v) {
}

optional<string> Integer::repr() { return to_string(value); }

ObjectOrError Integer::execute(Operation operation, Object* other) {
    if (auto otherVal = other->intValue(); otherVal.has_value())
        return Util::Obj::result(operation, this->value, otherVal.value());

    return make_unique<Error>(ErrorType::ConversionToIntFailed);
}

optional<int> Integer::intValue() { return value; }

optional<double> Integer::doubleValue() { return value; }

void Integer::erase() {
    Objects::remove(value);
}

optional<bool> Integer::booleanValue() {
    return value;
}