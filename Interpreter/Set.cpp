#include "Set.h"
#include "Objects.h"
#include <algorithm>
#include <iterator>

#include <sstream>

using namespace std;

Set::Set(std::vector<Object*>&& objects) {
    copy(cbegin(objects), cend(objects), inserter(this->value, this->value.begin()));
}

std::optional<std::string> Set::repr() {
    auto oss = ostringstream {};
    oss << "{";
    if (!value.empty()) {
        auto objects = vector<Object*>(cbegin(value), cend(value));
        transform(cbegin(objects), prev(cend(objects)), ostream_iterator<string>(oss, ", "),
                  [](Object* o) -> string { return o->repr().value(); });
        oss << objects.back()->repr().value();
    }
    oss << "}";
    return oss.str();
}

ObjectOrError Set::runMethod(const std::string_view s, const std::vector<Object*>& arguments) {
    if (s == "size") {
        if (arguments.empty()) {
            return Objects::getOrCreate((int) value.size());
        }
    }
    if (s == "contains") {
        if (arguments.size() == 1) {
            auto arg = arguments[0]->unref();
            if (isObject(arg)) {
                return Objects::get(
                    find(cbegin(value), cend(value), get<Object*>(arg)) != cend(value)
                );
            }
        }
    }
    if (s == "insert") {
        if (arguments.size() == 1) {
            auto arg = arguments[0]->unref();
            if (isObject(arg)) {
                auto argObj = get<Object*>(arg);
                value.insert(argObj);
                argObj->incrementReferenceCounter();
                return nullptr;
            }
        }
    }
    if (s == "remove") {
        if (arguments.size() == 1) {
            if (auto arg = arguments[0]->unref(); isObject(arg)) {
                auto argObj = get<Object*>(arg);
                auto findResult = find(begin(value), end(value), argObj);
                if (findResult != end(value)) {
                    value.erase(findResult);
                    argObj->decrementAndDestroy();
                }
                return nullptr;
            }
        }
    }

    return make_unique<Error>(ErrorType::UnknownError);
}
