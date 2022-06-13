#pragma once

#include "Node.h"

class DotNode : public Node {

    public:
    DotNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;
};

