#pragma once

#include "Node.h"

class BasicNode : public Node {
    Operation operation;
    public:
    BasicNode(int priority, Node* parent, Operation operation);

    virtual ObjectOrError calculate() override;

    Operation getOperation();
};

