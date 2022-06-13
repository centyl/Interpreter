#include "ReturnNode.h"
#include "TransformUtil.h"

using namespace std;

/*
* Used to return value from function
* Object returned is dereferenced, so that return(x) returns current value of x,
* not a reference to variable stored in a frame about to be removed from call stack
*/
ObjectOrError ReturnNode::calculate() {
    if (this->operandCount() == 1) {
        auto x = Util::Transform::toObject(*this->getOperand(0));
        return (isObject(x) ? get<Object*>(x)->unref() : move(x));
    }
    return make_unique<Error>(ErrorType::InvalidOperandCount);
}

ReturnNode::ReturnNode(int priority, Node* parent) : Node(priority, parent) {

}