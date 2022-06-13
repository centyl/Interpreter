#pragma once

#include "Node.h"

class PrintNode : public Node {

    public:
    PrintNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;
};