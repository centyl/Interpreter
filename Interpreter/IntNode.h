#pragma once

#include "Node.h"

class IntNode : public Node {

    public:
    IntNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;
};