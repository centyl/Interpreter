#include "Tree.h"
#include "Parser.h"

#include "IfNode.h"
#include "WhileNode.h"
#include "CommaNode.h"
#include "FunctionNode.h"
#include "ReturnNode.h"
#include "OrNode.h"
#include "AndNode.h"
#include "DotNode.h"
#include "BasicNode.h"
#include "ConstructorNode.h"
#include "CharNode.h"

#include "IntNode.h"
#include "DoubleNode.h"
#include "PrintNode.h"
#include "SqrtNode.h"
#include "SetNode.h"
#include "DictNode.h"

#include "TransformUtil.h"

using namespace std;

unique_ptr<Node> createNode(int priority, Operation operation, Node* parent = nullptr) {
    switch (operation) {
        case Operation::If:
            return make_unique<IfNode>(priority, parent);
        case Operation::While:
            return make_unique<WhileNode>(priority, parent);
        case Operation::Comma:
            return make_unique<CommaNode>(priority, parent);
        case Operation::Function:
            return make_unique<FunctionNode>(priority, parent);
        case Operation::ConvertToInt:
            return make_unique<IntNode>(priority, parent);
        case Operation::ConvertToDouble:
            return make_unique<DoubleNode>(priority, parent);
        case Operation::Print:
            return make_unique<PrintNode>(priority, parent);
        case Operation::SquareRoot:
            return make_unique<SqrtNode>(priority, parent);
        case Operation::Constructor:
            return make_unique<ConstructorNode>(priority, parent);
        case Operation::Return:
            return make_unique<ReturnNode>(priority, parent);
        case Operation::Or:
            return make_unique<OrNode>(priority, parent);
        case Operation::And:
            return make_unique<AndNode>(priority, parent);
        case Operation::Dot:
            return make_unique<DotNode>(priority, parent);
        case Operation::Char:
            return make_unique<CharNode>(priority, parent);
        case Operation::Set:
            return make_unique<SetNode>(priority, parent);
        case Operation::Dict:
            return make_unique<DictNode>(priority, parent);
        default:
            return make_unique<BasicNode>(priority, parent, operation);
    }
}

Node* anchorPoint(Node* node, int priority) {
    while (node->getParent() && node->getParent()->getPriority() >= priority) node = node->getParent();
    return node;
}

Node* Tree::getTopNode() {
    return topNode.get();
}

void Tree::addChildNode(int priority, Operation operation, optional<Operand>&& operand) {
    auto n = createNode(priority, operation, currentNode);

    if (operand.has_value())
        n->add(move(operand.value()));

    auto n_ptr = n.get();
    currentNode->add(move(n));
    currentNode = n_ptr;
}

void Tree::addUpperNode(Node* node, int priority, Operation operation) {
    auto nodeParent = node->getParent();
    auto node_uptr = (nodeParent
                      ? get<unique_ptr<Node>>(nodeParent->pop_back())
                      : move(topNode));

    auto newNode = createNode(priority, operation, nodeParent);
    newNode->add(move(node_uptr));

    currentNode = newNode.get();

    node->setParent(newNode);

    if (nodeParent) nodeParent->add(move(newNode));
    else topNode = move(newNode);
}

void Tree::addNode(int priority, Operation operation, optional<Operand>&& operand) {
    if (!currentNode) {
        topNode = createNode(priority, operation);

        if (operand.has_value())
            topNode->add(move(operand.value()));

        currentNode = topNode.get();
    }
    else {
        if (priority == currentNode->getPriority()) {
            if (!Parser::isAssignment(priority)) {
                if (operand.has_value())
                    currentNode->add(move(operand.value()));

                if (operation != Operation::Comma)
                    addUpperNode(currentNode, priority, operation);
            }
            else addChildNode(priority, operation, move(operand));
        }
        else if (priority > currentNode->getPriority())
            addChildNode(priority, operation, move(operand));
        else {
            if (operand.has_value())
                currentNode->add(move(operand.value()));

            auto anchor = anchorPoint(currentNode, priority);

            if (!dynamic_cast<CommaNode*>(anchor) || operation != Operation::Comma)
                addUpperNode(anchor, priority, operation);
            else currentNode = anchor;
        }
    }
}

void Tree::addNode(int priority, Operation operation, std::string_view sv) {
    if (!sv.empty()) {
        auto object = Util::Transform::toObject(sv);
        addNode(priority, operation, object);
        object->incrementReferenceCounter();
    }
    else addNode(priority, operation, this->currentNode->pop_back());
}

void Tree::add(string_view sv) {
    currentNode->add(sv);
}

ObjectOrError Tree::calculate() {
    return (topNode ? topNode->calculate() : make_unique<Error>(ErrorType::UnknownError));
}

Node* Tree::getCurrentNode() {
    return currentNode;
}

void Tree::setCurrentNode(Node* node) {
    currentNode = node;
}
