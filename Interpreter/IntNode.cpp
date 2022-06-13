#include "IntNode.h"
#include "Objects.h"
#include "TransformUtil.h"

using namespace std;

ObjectOrError IntNode::calculate() {
    if (this->operandCount() == 1) {
        auto argument = Util::Transform::toObject(*this->getOperand(0));
        if (isObject(argument)) {
            if (auto obj = get<Object*>(argument)) {
                if (auto i = obj->intValue(); i.has_value()) {
                    return Objects::getOrCreate(i.value());
                }
                return make_unique<Error>(ErrorType::UnknownError);
            }
            return make_unique<Error>(ErrorType::UnknownError);
        }
        return argument;
    }
    return make_unique<Error>(ErrorType::InvalidOperandCount);
}

IntNode::IntNode(int priority, Node* parent) : Node(priority, parent) {

}