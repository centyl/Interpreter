#pragma once

#include "Node.h"

class DictNode :
    public Node {
    public:
    DictNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;
};

