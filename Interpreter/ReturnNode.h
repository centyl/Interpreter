#pragma once

#include "Node.h"

class ReturnNode : public Node {

    public:
    ReturnNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;
};