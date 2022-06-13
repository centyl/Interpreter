#include "CustomObject.h"
#include "FunctionUtil.h"
#include "Reference.h"
#include "ReferenceStack.h"
#include "TransformUtil.h"
#include "Objects.h"
#include "Functions.h"
#include "NullObject.h"

using namespace std;

// calls object's str method and returns its value, or returns class name if no such method is available
optional<string> CustomObject::repr() {
    if (auto f = Functions::find(className, "str", 0)) {
        ReferenceStack::push();
        ReferenceStack::set("this", this);
        auto res = get<Object*>(f->second->calculate())->repr();
        ReferenceStack::pop();
        return res;
    }
    return className;
}

ObjectOrError CustomObject::runMethod(const string_view s, const vector<Object*>& arguments) {
    if (s == "set") { // obj.set(x, 5) sets x field of obj to 5
        if (arguments.size() == 2) {
            if (auto field = dynamic_cast<Reference*>(arguments[0])) {
                if (auto un = arguments[1]->unref(); isObject(un)) {
                    auto fieldName = field->getName();
                    auto obj = get<Object*>(un);
                    fields[string(fieldName)] = obj;
                    obj->incrementReferenceCounter();
                    return Objects::getNullObject();
                }
                else return un;
            }
            return make_unique<Error>(ErrorType::UnknownError);
        }
        return make_unique<Error>(ErrorType::InvalidArgumentCount);
    }
    if (auto method = Functions::find(this->className, s, arguments.size())) { // calling custom function defined in source file
        auto unrefd = Util::Transform::toUnreferencedObjects(cbegin(arguments), cend(arguments)); // get arg objects
        if (isObject(unrefd)) {
            Util::Function::pushArguments(method->first, get<vector<Object*>>(unrefd)); // push frame to reference stack, initialize arg variables
            ReferenceStack::set("this", this); // add 'this' reference, so that method can access other members
            auto res = method->second->calculate(); // execute method's body
            ReferenceStack::pop(); // remove frame
            return res;
        }
        return move(get<unique_ptr<Error>>(unrefd));
    }
    if (arguments.empty()) { // obj.x may refer to field x, not method x
        if (auto l = this->fields.find(s); l != this->fields.end()) {
            return l->second;
        }
    }
    return std::make_unique<Error>(ErrorType::UnknownError);
}

void CustomObject::erase() {
    for (auto field : fields) {
        if (auto obj = field.second) {
            obj->decrementAndDestroy();
        }
    }
    delete this;
}

CustomObject::CustomObject(const std::string_view className) : className(className) {

}