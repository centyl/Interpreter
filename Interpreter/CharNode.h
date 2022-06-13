#pragma once

#include "Node.h"

class CharNode : public Node {

    public:
    CharNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;
};