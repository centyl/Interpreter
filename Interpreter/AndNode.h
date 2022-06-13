#pragma once

#include "Node.h"

class AndNode : public Node {

    public:
    AndNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;
};

