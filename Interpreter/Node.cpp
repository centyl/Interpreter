#include "Node.h"

#include "Node.h"
#include "TransformUtil.h"

using namespace std;

Operand Node::pop_back() {
    auto result = move(operands.back());
    operands.pop_back();
    return result;
}

vector<Operand>::const_iterator Node::front() {
    return cbegin(operands);
}

vector<Operand>::const_iterator Node::back() {
    return cend(operands);
}

int Node::operandCount() {
    return operands.size();
}

int Node::getPriority() {
    return priority;
}

Operand* Node::getOperand(int i) {
    return addressof(operands[i]);
}

Operand* Node::lastOperand() {
    return addressof(operands.back());
}

void Node::add(Operand&& operand) {
    operands.emplace_back(move(operand));
}

void Node::add(std::string_view sv) {
    auto o = Util::Transform::toObject(sv);
    add(o);
    o->incrementReferenceCounter();
}

Node::Node(int priority, Node* parent) : priority(priority), parent(parent) {

}

void Node::setParent(std::unique_ptr<Node>& parent) {
    this->parent = parent.get();
}

Node* Node::getParent() {
    return this->parent;
}