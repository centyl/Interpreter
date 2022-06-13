#include "SetNode.h"
#include "TransformUtil.h"
#include "Array.h"
#include "Set.h"

using namespace std;

SetNode::SetNode(int priority, Node* parent) : Node(priority, parent) {
}

ObjectOrError SetNode::calculate() {
    if (this->operandCount() == 0) {
        return new Set({});
    }
    if (this->operandCount() == 1) {
        auto op = this->getOperand(0);
        if (holds_alternative<unique_ptr<Node>>(*op)) {
            auto node = get<unique_ptr<Node>>(*op).get();
            auto objects = Util::Transform::toUnreferencedObjects(node->front(), node->back());
            if (isObject(objects)) {
                auto objs = get<vector<Object*>>(objects);
                return new Set(move(objs));
            }
        }
    }
    return make_unique<Error>(ErrorType::UnknownError);
}
