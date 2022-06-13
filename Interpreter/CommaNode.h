#pragma once

#include "Node.h"

class CommaNode : public Node {

    public:
    CommaNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;
};