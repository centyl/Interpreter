#include "OrNode.h"

#include "Objects.h"
#include "TransformUtil.h"

using namespace std;

// return logical disjunction of two operands, using short circuit evaluation
// if boolean value of the first operand is true, value of the second one is not calculated
ObjectOrError OrNode::calculate() {
    if (this->operandCount() == 2) {
        if (auto left = Util::Transform::toObject(*this->getOperand(0)); isObject(left)) {
            if (auto leftVal = get<Object*>(left)->booleanValue(); leftVal.has_value()) {
                if (leftVal.value())
                    return Objects::get(true);

                if (auto right = Util::Transform::toObject(*this->getOperand(1)); isObject(right)) {
                    auto rightVal = get<Object*>(right)->booleanValue();
                    return (rightVal.has_value()
                            ? Objects::get(rightVal.value())
                            : (ObjectOrError) make_unique<Error>(ErrorType::UnknownError));
                }
            }
            return make_unique<Error>(ErrorType::UnknownError);
        }
        return make_unique<Error>(ErrorType::UnknownError);
    }
    return make_unique<Error>(ErrorType::InvalidOperandCount);
}

OrNode::OrNode(int priority, Node* parent) : Node(priority, parent) {

}