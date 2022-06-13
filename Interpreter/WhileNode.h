#pragma once

#include "Node.h"

class WhileNode : public Node {

    public:
    WhileNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;

};