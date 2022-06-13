#include "Array.h"
#include "CommaNode.h"
#include "TransformUtil.h"

using namespace std;

// return Array object initialized with node's operands
ObjectOrError CommaNode::calculate() {
    // operands are converted to objects and dereferenced, so that when creating (x, y, z) array, current values of those variables are used,
    // not the references, which could later change their values, themselves
    auto objects = Util::Transform::toUnreferencedObjects(this->front(), this->back());
    return (isObject(objects)
            ? new Array(move(get<vector<Object*>>(objects)))
            : (ObjectOrError) move(get<unique_ptr<Error>>(objects)));
}

CommaNode::CommaNode(int priority, Node* parent) : Node(priority, parent) {

}