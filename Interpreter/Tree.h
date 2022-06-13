#pragma once

#include <memory>
#include <optional>

#include "Operation.h"
#include "Node.h"

class Block;
class Node;
class Object;

using Operand = std::variant<Object*, std::unique_ptr<Node>, std::unique_ptr<Block>>;

class Tree {
    Node* currentNode;

    std::unique_ptr<Node> topNode;

    void addChildNode(int priority, Operation operation, std::optional<Operand>&& operand);

    void addUpperNode(Node* node, int priority, Operation operation);
    public:

    Node* getTopNode();

    void addNode(int priority, Operation operation, std::optional<Operand>&& operand = std::nullopt);

    void addNode(int priority, Operation operation, std::string_view sv);

    void add(std::string_view sv);

    ObjectOrError calculate();

    Node* getCurrentNode();

    void setCurrentNode(Node* node);
};