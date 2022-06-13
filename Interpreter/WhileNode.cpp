#include <variant>

#include "WhileNode.h"
#include "Objects.h"
#include "TransformUtil.h"

using namespace std;

/*
* Executes loop as long as the predicate is true
* Also breaks when return call or an error occurs inside the loop
*/
ObjectOrError WhileNode::calculate() {
    if (this->operandCount() == 2) {
        while (true) {
            auto b = Util::Transform::toObject(*this->getOperand(0));
            if (isObject(b)) {
                auto bv = get<Object*>(b)->booleanValue();
                if (bv.has_value()) {
                    if (!bv.value())
                        return Objects::getNullObject();

                    auto res = get<unique_ptr<Block>>(*this->getOperand(1))->calculate();
                    if ((isObject(res) && get<Object*>(res) != Objects::getNullObject()) || isError(res))
                        return res;
                }
                else return make_unique<Error>(ErrorType::UnknownError);
            }
            else return b;
        }
    }
    return make_unique<Error>(ErrorType::UnknownError);
}

WhileNode::WhileNode(int priority, Node* parent) : Node(priority, parent) {

}