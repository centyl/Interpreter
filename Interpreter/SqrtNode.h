#pragma once

#include "Node.h"

class SqrtNode : public Node {

    public:
    SqrtNode(int priority, Node* parent);

    virtual ObjectOrError calculate() override;
};