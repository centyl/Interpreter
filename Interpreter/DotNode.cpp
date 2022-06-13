#include "DotNode.h"
#include "Reference.h"
#include "CommaNode.h"
#include "BasicNode.h"

#include "TransformUtil.h"

#include <algorithm>

using namespace std;

/*
* run method of object provided as 1st argument and return its result
*/
ObjectOrError DotNode::calculate() {
    if (this->operandCount() == 2) {
        if (auto left = Util::Transform::toObject(*this->getOperand(0)); isObject(left)) {
            auto leftObject = get<Object*>(left);
            if (holds_alternative<Object*>(*this->getOperand(1))) { // 0 arg function, e.g. a.size
                auto ref = dynamic_cast<Reference*>(get<Object*>(*this->getOperand(1)));
                return (ref ? leftObject->runMethod(ref->getName()) : make_unique<Error>(ErrorType::UnknownError));
            }

            if (holds_alternative<unique_ptr<Node>>(*this->getOperand(1))) {
                auto node = get<unique_ptr<Node>>(*this->getOperand(1)).get();
                if (node->operandCount() == 1 || node->operandCount() == 2) {
                    if (auto ref = dynamic_cast<Reference*>(get<Object*>(*node->getOperand(0)))) {
                        auto name = ref->getName();

                        if (node->operandCount() == 1) // 0 arg function, e.g. a.length()
                            return leftObject->runMethod(name);

                        if (holds_alternative<Object*>(*node->getOperand(1))) // 1 object argument, e.g. a.count(2)
                            return leftObject->runMethod(name, get<Object*>(*node->getOperand(1)));

                        if (holds_alternative<unique_ptr<Node>>(*node->getOperand(1))) {
                            auto n = get<unique_ptr<Node>>(*node->getOperand(1)).get();
                            if (auto comma = dynamic_cast<CommaNode*>(n)) { // multiple arguments, e.g. a.sub(2,3)
                                auto objectPointers = Util::Transform::toObjects(n->front(), n->back());
                                return (isObject(objectPointers)
                                        ? leftObject->runMethod(name, get<vector<Object*>>(objectPointers))
                                        : move(get<unique_ptr<Error>>(objectPointers)));
                            }

                            if (auto basic = dynamic_cast<BasicNode*>(n)) {
                                if (basic->getOperation() == Operation::Lambda) { // lambda argument, e.g. a.count(x->x<5)
                                    if (basic->operandCount() == 2) {
                                        if (holds_alternative<Object*>(*n->getOperand(0))) {
                                            if (auto refn = dynamic_cast<Reference*>(get<Object*>(*n->getOperand(0)))) {
                                                return (holds_alternative<unique_ptr<Node>>(*n->getOperand(1))
                                                        ? leftObject->runMethod(name, refn->getName(),
                                                                                get<unique_ptr<Node>>(*n->getOperand(1)).get())
                                                        : make_unique<Error>(ErrorType::InvalidLambdaBody));

                                            }
                                        }
                                        return make_unique<Error>(ErrorType::InvalidLambdaArgument);
                                    }
                                    return make_unique<Error>(ErrorType::InvalidOperandCount);
                                }
                            }

                            // 1 argument, has to be calculated first, e.g. a.count(2 + 6)
                            auto arg = n->calculate();
                            return (isObject(arg)
                                    ? leftObject->runMethod(name, get<Object*>(arg))
                                    : move(get<unique_ptr<Error>>(arg)));

                        }
                        return leftObject->runMethod(name);
                    }
                }
                return make_unique<Error>(ErrorType::UnknownError);
            }
        }
        else return left;
    }
    return make_unique<Error>(ErrorType::InvalidOperandCount);
}

DotNode::DotNode(int priority, Node* parent) : Node(priority, parent) {

}