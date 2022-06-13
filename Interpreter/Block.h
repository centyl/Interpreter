#pragma once

#include "Tree.h"

class Tree;

class Block {
    std::vector<std::unique_ptr<Tree>> lines;

    public:
    ObjectOrError calculate();

    Tree* lastLine();

    void addLine(std::unique_ptr<Tree> line);
};