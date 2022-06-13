#include <cmath>

#include "SqrtNode.h"
#include "Objects.h"
#include "TransformUtil.h"

using namespace std;

ObjectOrError SqrtNode::calculate() {
    if (this->operandCount() == 1) {
        auto argument = Util::Transform::toObject(*this->getOperand(0));
        if (isObject(argument)) {
            if (auto obj = get<Object*>(argument)) {
                if (auto d = obj->doubleValue(); d.has_value())
                    return Objects::getOrCreate(std::sqrt(d.value()));

                if (auto i = obj->intValue(); i.has_value())
                    return Objects::getOrCreate(std::sqrt(i.value()));

                return make_unique<Error>(ErrorType::UnknownError);
            }
            return make_unique<Error>(ErrorType::UnknownError);
        }
        return argument;
    }
    return make_unique<Error>(ErrorType::InvalidOperandCount);
}

SqrtNode::SqrtNode(int priority, Node* parent) : Node(priority, parent) {

}