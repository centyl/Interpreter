#include "BasicNode.h"
#include "TransformUtil.h"

using namespace std;

// return result of performing binary operation on two operands
// this node is responsible for operations such as addition or comparison
ObjectOrError BasicNode::calculate() {
    if (this->operandCount() == 2) {
        if (auto left = Util::Transform::toObject(*this->getOperand(0)); isObject(left)) {
            if (auto right = Util::Transform::toObject(*this->getOperand(1)); isObject(right)) {
                auto result = get<Object*>(left)->execute(this->operation, get<Object*>(right));
                if (isObject(result)) {
                    auto leftObject = get<Object*>(left);
                    auto rightObject = get<Object*>(right);
                    auto resultObject = get<Object*>(result);
                    if (leftObject != resultObject) leftObject->destructIfNotReferenced();
                    if (rightObject != resultObject) rightObject->destructIfNotReferenced();
                }
                return result;
            }
            else return right;
        }
        else return left;
    }
    return make_unique<Error>(ErrorType::InvalidOperandCount);
}

BasicNode::BasicNode(int priority, Node* parent, Operation operation) : Node(priority, parent), operation(operation) {

}

Operation BasicNode::getOperation() {
    return operation;
}