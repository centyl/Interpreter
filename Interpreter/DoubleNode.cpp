#include "DoubleNode.h"
#include "Objects.h"
#include "TransformUtil.h"

using namespace std;

ObjectOrError DoubleNode::calculate() {
    if (this->operandCount() == 1) {
        auto argument = Util::Transform::toObject(*this->getOperand(0));
        if (isObject(argument)) {
            if (auto obj = get<Object*>(argument)) {
                auto d = obj->doubleValue();
                return (d.has_value()
                        ? Objects::getOrCreate(d.value())
                        : (ObjectOrError) make_unique<Error>(ErrorType::UnknownError));
            }
            return make_unique<Error>(ErrorType::UnknownError);
        }
        return argument;
    }
    return make_unique<Error>(ErrorType::InvalidOperandCount);
}

DoubleNode::DoubleNode(int priority, Node* parent) : Node(priority, parent) {

}