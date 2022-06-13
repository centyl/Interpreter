#include "Block.h"
#include "ReturnNode.h"
#include "IfNode.h"
#include "WhileNode.h"
#include "Objects.h"

using namespace std;

ObjectOrError Block::calculate() {
    for (auto& line : lines) {
        if (auto result = line->calculate(); isObject(result)) {
            auto topNode = line->getTopNode();
            if (dynamic_cast<ReturnNode*>(topNode) ||
                ((dynamic_cast<IfNode*>(topNode) || dynamic_cast<WhileNode*>(topNode)) // if return() call occurred, executing current block should be halted
                 && get<Object*>(result) != Objects::getNullObject())) return result;
        }
        else { // error occurred
            return result;
        }
    }
    return Objects::getNullObject(); // if return() was not called, block should return null
}

Tree* Block::lastLine() {
    return (!lines.empty() ? lines.back().get() : nullptr);
}

void Block::addLine(std::unique_ptr<Tree> line) {
    lines.push_back(move(line));
}
