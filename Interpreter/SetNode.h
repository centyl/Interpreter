#pragma once

#include <set>

#include "Node.h"

class SetNode :
    public Node {
    public:
    SetNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;
};

