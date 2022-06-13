#include "AndNode.h"
#include "Objects.h"
#include "TransformUtil.h"

using namespace std;

// return logical conjunction of two operands, using short circuit evaluation
// if boolean value of the first operand is false, value of the second one is not calculated
ObjectOrError AndNode::calculate() {
    if (this->operandCount() == 2) {
        if (auto left = Util::Transform::toObject(*this->getOperand(0)); isObject(left)) {
            auto leftObj = get<Object*>(left);
            if (auto leftVal = leftObj->booleanValue(); leftVal.has_value()) {
                if (!leftVal.value())
                    return Objects::get(false);

                if (auto right = Util::Transform::toObject(*this->getOperand(1)); isObject(right)) {
                    auto rightObj = get<Object*>(right);
                    auto rightVal = rightObj->booleanValue();

                    leftObj->destructIfNotReferenced();
                    rightObj->destructIfNotReferenced();

                    return (rightVal.has_value()
                            ? Objects::get(rightVal.value())
                            : (ObjectOrError) make_unique<Error>(ErrorType::NoBooleanValue));
                }
                else return right;
            }
            return make_unique<Error>(ErrorType::NoBooleanValue);
        }
        else return left;
    }
    return make_unique<Error>(ErrorType::InvalidOperandCount);
}

AndNode::AndNode(int priority, Node* parent) : Node(priority, parent) {

}