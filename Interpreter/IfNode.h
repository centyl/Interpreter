#pragma once

#include "Node.h"

class IfNode : public Node {

    public:
    IfNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;
};