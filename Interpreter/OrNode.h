#pragma once

#include "Node.h"

class OrNode : public Node {

    public:
    OrNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;

};