#pragma once

#include "Node.h"

class FunctionNode : public Node {

    public:
    FunctionNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;
};