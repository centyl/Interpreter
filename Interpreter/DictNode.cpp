#include "DictNode.h"
#include "Dict.h"

using namespace std;

ObjectOrError DictNode::calculate() {
    return new Dict();
}

DictNode::DictNode(int priority, Node* parent) : Node(priority, parent) {
}