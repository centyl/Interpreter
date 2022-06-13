#pragma once

#include "Node.h"

class ConstructorNode : public Node {

    public:
    ConstructorNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;
};

