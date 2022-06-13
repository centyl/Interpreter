#include <iostream>
#include <string>

#include "PrintNode.h"
#include "Objects.h"
#include "TransformUtil.h"

using namespace std;

ObjectOrError PrintNode::calculate() {
    if (this->operandCount() == 1) {
        auto argument = Util::Transform::toObject(*this->getOperand(0));
        if (isObject(argument)) {
            if (auto obj = get<Object*>(argument)) {
                cout << obj->repr().value() << endl;
                obj->destructIfNotReferenced();
                return Objects::getNullObject();
            }
            return make_unique<Error>(ErrorType::UnknownError);
        }
        return argument;
    }
    return make_unique<Error>(ErrorType::InvalidOperandCount);
}

PrintNode::PrintNode(int priority, Node* parent) : Node(priority, parent) {

}