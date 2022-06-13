#pragma once

#include "Node.h"

class DoubleNode : public Node {

    public:
    DoubleNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;
};