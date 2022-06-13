#include "Dict.h"
#include "String.h"
#include "Objects.h"
#include <sstream>
#include <algorithm>

using namespace std;

ObjectOrError Dict::runMethod(const std::string_view s, const std::vector<Object*>& arguments) {
    if (s == "set") { // analogous to setting custom object's fields
        if (arguments.size() == 2) {
            if (auto field = dynamic_cast<String*>(arguments[0])) {
                if (auto un = arguments[1]->unref(); isObject(un)) {
                    auto fieldName = field->repr().value();
                    auto obj = get<Object*>(un);
                    objects[fieldName] = obj;
                    obj->incrementReferenceCounter();
                    return Objects::getNullObject();
                }
                else return un;
            }
        }
    }
    if (s == "get") {
        if (arguments.size() == 1) {
            if (auto field = dynamic_cast<String*>(arguments[0])) {
                auto fieldName = field->repr().value();
                return objects[fieldName];
            }
        }
    }

    return make_unique<Error>(ErrorType::UnknownError);
}

std::optional<std::string> Dict::repr() {
    auto oss = ostringstream {};
    oss << "{";
    for (auto [key, value] : objects) {
        oss << "'" << key << "': " << value->repr().value() << ", ";
    }
    oss << "}";
    return oss.str();
}
