#pragma once

#include "Object.h"
#include "Block.h"
#include "Error.h"

class Block;

using Operand = std::variant<Object*, std::unique_ptr<Node>, std::unique_ptr<Block>>;

class Node {
    int priority;
    std::vector<Operand> operands;
    Node* parent;
    public:

    Operand pop_back();

    std::vector<Operand>::const_iterator front();

    std::vector<Operand>::const_iterator back();

    int operandCount();

    int getPriority();

    Operand* getOperand(int i);

    Operand* lastOperand();

    void add(Operand&& operand);

    void add(std::string_view sv);

    Node(int priority, Node* parent);

    void setParent(std::unique_ptr<Node>& parent);

    Node* getParent();

    virtual ObjectOrError calculate() = 0;

};