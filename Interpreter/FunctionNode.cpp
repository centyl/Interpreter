#include <iostream>

#include "FunctionNode.h"
#include "Functions.h"
#include "Reference.h"
#include "ReferenceStack.h"
#include "Objects.h"
#include "CustomObject.h"
#include "Array.h"
#include <cctype>

#include "FunctionUtil.h"

using namespace std;
using namespace Util::Function;

/*
* First operand should contain name of the global function to be called
* After a global function with correct name and number of arguments is found
*   - a new reference frame is pushed onto the reference stack
*   - for each argument name in function's definition, a variable is created
*   - body of the function is executed
*   - frame is removed from the stack, making sure not to deallocate object returned by the function
*   - return value returned by the function's body
*/
ObjectOrError FunctionNode::calculate() {
    if (this->operandCount()) {
        if (auto ref = dynamic_cast<Reference*>(std::get<Object*>(*this->getOperand(0)))) {
            auto functionName = ref->getName();
            if (auto function = Functions::findGlobal(functionName, getArgumentCount(this->front(), this->back()))) {
                if (auto objs = getArguments(this->front(), this->back()); isObject(objs)) {
                    pushArguments(function->first, get<vector<Object*>>(objs));
                    auto functionResult = function->second->calculate();
                    if (isObject(functionResult)) {
                        ReferenceStack::pop(get<Object*>(functionResult));
                    }
                    else ReferenceStack::pop();
                    return functionResult;
                }
                else return move(get<unique_ptr<Error>>(objs));
            }
        }
        return make_unique<Error>(ErrorType::UnknownError);
    }
    return make_unique<Error>(ErrorType::InvalidOperandCount);
}

FunctionNode::FunctionNode(int priority, Node* parent) : Node(priority, parent) {

}