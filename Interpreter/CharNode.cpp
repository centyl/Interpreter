#include "CharNode.h"
#include "String.h"
#include "TransformUtil.h"

using namespace std;

// return char interpretation of Object's integer value
ObjectOrError CharNode::calculate() {
    if (this->operandCount() == 1) {
        auto argument = Util::Transform::toObject(*this->getOperand(0));
        if (isObject(argument)) {
            if (auto obj = get<Object*>(argument)) {
                if (auto i = obj->intValue(); i.has_value()) {
                    char c[2];
                    c[0] = i.value();
                    c[1] = 0;
                    return new String(c);
                }
            }
        }
    }
    return make_unique<Error>(ErrorType::InvalidOperandCount);
}

CharNode::CharNode(int priority, Node* parent) : Node(priority, parent) {

}