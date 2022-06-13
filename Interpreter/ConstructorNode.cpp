#include <algorithm>

#include "ConstructorNode.h"
#include "CustomObject.h"
#include "FunctionUtil.h"
#include "Reference.h"
#include "TransformUtil.h"
#include "Functions.h"
#include "ReferenceStack.h"

using namespace std;
using namespace Util::Function;
using namespace Util::Transform;

/*
* When constructing an object, first operand must contain name of the class to be instanced
* If constructor taking correct number of arguments is found, a function call occurs:
*   - a new reference frame is pushed onto the reference stack
*   - for each argument name in constructor's definition, a variable is created
*   - variable 'this' is created, pointing to newly created custom object
*   - body of the constructor function is executed
*   - frame is removed from the stack, custom object is returned
*/
ObjectOrError ConstructorNode::calculate() {
    if (this->operandCount() && holds_alternative<Object*>(*this->getOperand(0))) {
        if (auto ref = dynamic_cast<Reference*>(get<Object*>(*this->getOperand(0)))) {
            auto className = ref->getName();
            if (auto constructor = Functions::findConstructor(className, getArgumentCount(this->front(), this->back()))) {
                if (auto arguments = getArguments(this->front(), this->back()); isObject(arguments)) {
                    auto args = get<vector<Object*>>(arguments);
                    pushArguments(constructor->first, get<vector<Object*>>(arguments));
                    auto obj = new CustomObject(className);
                    ReferenceStack::set("this", obj);
                    constructor->second->calculate();
                    ReferenceStack::pop(obj);
                    return obj;
                }
                else return move(get<unique_ptr<Error>>(arguments));
            }
            return make_unique<Error>(ErrorType::ConstructorNotFound);
        }
    }
    return make_unique<Error>(ErrorType::UnknownError);
}

ConstructorNode::ConstructorNode(int priority, Node* parent) : Node(priority, parent) {

}