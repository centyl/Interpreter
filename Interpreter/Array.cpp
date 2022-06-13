#include <algorithm>
#include <iterator>
#include <sstream>
#include <variant>

#include "Node.h"
#include "Array.h"
#include "Objects.h"
#include "ReferenceStack.h"

using namespace std;

Array::Array(vector<Object*>&& objects) : objects(objects) {
    for (auto object : objects)
        object->incrementReferenceCounter();
}

optional<string> Array::repr() {
    auto oss = ostringstream {};
    oss << "[";
    if (!objects.empty()) {
        transform(cbegin(objects), prev(cend(objects)), ostream_iterator<string>(oss, ", "),
                  [](Object* o) -> string { return o->repr().value(); });
        oss << objects.back()->repr().value();
    }
    oss << "]";
    return oss.str();
}

ObjectOrError Array::execute(Operation operation, Object* other) {
    if (operation == Operation::Add) { // (1,2) + 3 = (1,2,3)
        auto obj = std::get<Object*>(other->unref());
        objects.push_back(obj);
        obj->incrementReferenceCounter();
        return this;
    }
    return make_unique<Error>(ErrorType::UnknownError);
}

void Array::erase() {
    for (auto object : objects) {
        object->decrementAndDestroy();
    }
    delete this;
}

optional<bool> Array::booleanValue() {
    return !objects.empty();
}

ObjectOrError Array::runMethod(const string_view s, const vector<Object*>& arguments) {
    if (s == "length") { // (1,2,3).length = (4,5,6).length() = 3
        if (arguments.empty())
            return Objects::getOrCreate((int) objects.size());
        return make_unique<Error>(ErrorType::UnknownError);
    }
    if (s == "contains") // (1,2,3).contains(2) = true; (1,2,3).contains(4) = false
        return Objects::getOrCreate(find(cbegin(objects), cend(objects), arguments[0]) != cend(objects));

    if (s == "count") // (1, 2, 3, 2).count(2) = 2
        return Objects::getOrCreate((int) count(cbegin(objects), cend(objects), arguments[0]));

    if (s == "filter") { // (1,2,3,2).filter(2)=(2,2)
        std::vector<Object*> filteredObjects;
        std::copy_if(cbegin(objects), cend(objects), std::back_inserter(filteredObjects),
                     [arguments](const Object* x) -> bool { return x == arguments[0]; });
        return new Array(move(filteredObjects));
    }
    if (s == "sub") { // (1,2,5,5,7).sub(1,4)=(2,5,5)
        vector<Object*> filteredObjects;
        if (!arguments[0]->intValue().has_value())
            return make_unique<Error>(ErrorType::UnknownError);
        if (!arguments[1]->intValue().has_value())
            return make_unique<Error>(ErrorType::UnknownError);
        copy(cbegin(objects) + (arguments[0]->intValue().value()), cbegin(objects) + (arguments[1]->intValue().value()),
             back_inserter(filteredObjects));
        return new Array(move(filteredObjects));
    }
    if (s == "at") { // (1,2,5,7,9).at(3) = 7
        return this->objects[arguments[0]->intValue().value()];
    }

    return make_unique<Error>(ErrorType::UnknownError);
}

ObjectOrError Array::runMethod(const string_view s, const string_view argumentName, Node* n) {
    if (s == "filter") { // (1,2,3,4,5,6).filter(x->x%2==0) = (2,4,6)
        vector<Object*> res;

        for (auto obj : objects) {
            ReferenceStack::push();
            ReferenceStack::set(argumentName, obj);
            if (auto result = n->calculate(); isObject(result)) {
                if (auto bv = std::get<Object*>(result)->booleanValue(); bv.has_value()) {
                    if (bv.value()) {
                        res.push_back(obj);
                        obj->incrementReferenceCounter();
                    }
                }
            }
            ReferenceStack::pop();
        }
        return new Array(move(res));
    }
    if (s == "map") { // (1,2,3,4,5,6).map(a->a*a) = (1,4,9,16,25,36)
        auto res = vector<Object*> {};
        for (auto obj : objects) {
            ReferenceStack::push();
            ReferenceStack::set(argumentName, obj);
            auto lambdaResult = n->calculate();
            if (isError(lambdaResult)) {
                ReferenceStack::pop();
                return move(get<unique_ptr<Error>>(lambdaResult));
            }
            auto o = get<Object*>(lambdaResult);
            if (!o) {
                ReferenceStack::pop();

                return make_unique<Error>(ErrorType::UnknownError);
            }
            res.push_back(o);
            ReferenceStack::pop();
        }
        return new Array(move(res));

    }
    if (s == "all") { // (1,2,3,4,5,6).all(x->x<10) = true, short circuits on false
        for (auto obj : objects) {
            ReferenceStack::push();
            ReferenceStack::set(argumentName, obj);
            auto b = n->calculate();
            ReferenceStack::pop();
            if (isError(b)) return b;
            if (auto x = std::get<Object*>(b)->booleanValue(); !x.has_value()) {
                return make_unique<Error>(ErrorType::UnknownError);
            }
            else if (!x.value())
                return Objects::getOrCreate(0);
        }
        return Objects::getOrCreate(1);
    }
    if (s == "none") { // (1,2,3,4,5,6).map(u->u*u).none(x->x>10) = false, short circuits on true
        for (auto obj : objects) {
            ReferenceStack::push();
            ReferenceStack::set(argumentName, obj);
            auto b = n->calculate();
            ReferenceStack::pop();
            if (isError(b)) return b;
            if (auto x = std::get<Object*>(b)->booleanValue(); !x.has_value()) {

                return make_unique<Error>(ErrorType::UnknownError);
            }
            else if (x.value())
                return Objects::getOrCreate(0);
        }
        return Objects::getOrCreate(1);
    }

    return make_unique<Error>(ErrorType::UnknownError);
}
