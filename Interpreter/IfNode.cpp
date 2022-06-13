#include "IfNode.h"
#include "Objects.h"
#include "TransformUtil.h"

using namespace std;

/*
* If first operand's boolean value is true, block stored as second operand is executed
* Otherwise, if there's a second (else) block, it is executed instead
*/
ObjectOrError IfNode::calculate() {
    if (this->operandCount() == 2 || this->operandCount() == 3) {
        if (auto expression = Util::Transform::toObject(*this->getOperand(0)); isObject(expression)) {
            if (auto exprValue = get<Object*>(expression)->booleanValue(); exprValue.has_value()) {
                if (exprValue.value())
                    return get<unique_ptr<Block>>(*this->getOperand(1))->calculate();

                if (this->operandCount() == 3)
                    return get<unique_ptr<Block>>(*this->getOperand(2))->calculate();

                return Objects::getNullObject();
            }
            return make_unique<Error>(ErrorType::UnknownError);
        }
        return make_unique<Error>(ErrorType::UnknownError);
    }
    return make_unique<Error>(ErrorType::InvalidOperandCount);
}

IfNode::IfNode(int priority, Node* parent) : Node(priority, parent) {

}