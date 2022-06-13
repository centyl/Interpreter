#include <variant>
#include <regex>
#include <map>
#include <stack>
#include <fstream>

#include "Parser.h"
#include "Reference.h"
#include "Functions.h"
#include "RegexUtil.h"
#include "TransformUtil.h"
#include "Dict.h"

#include <algorithm>

using namespace std;

namespace Parser {
    namespace {
        const static int priorityCount = 12;

        map<Operation, int> operationPriority {
            { Operation::Lambda, 0 },
            { Operation::Comma, 1 },
            { Operation::Assign, 2 },
            { Operation::Or, 3 },
            { Operation::And, 4 },
            { Operation::Equal,5 },
            { Operation::NotEqual,5 },
            { Operation::Greater, 6 },
            { Operation::GreaterEqual, 6 },
            { Operation::Less, 6 },
            { Operation::LessEqual, 6},
            { Operation::Add, 7 },
            { Operation::Subtract, 7 },
            { Operation::Multiply, 8 },
            { Operation::Divide, 8 },
            { Operation::FloorDivision, 8},
            { Operation::Mod, 8 },
            { Operation::Square, 9 },
            { Operation::Dot, 10 },
        };

        map<string, Operation, less<>> symbolToOperation {
            { "+"s, Operation::Add },
            { "-"s, Operation::Subtract },
            { "*"s, Operation::Multiply },
            { ">"s, Operation::Greater },
            { "=>"s, Operation::GreaterEqual },
            { "<"s, Operation::Less },
            { "<="s, Operation::LessEqual},
            { "**"s, Operation::Square },
            { "="s, Operation::Assign },
            { "=="s, Operation::Equal },
            { "!="s, Operation::NotEqual },
            { ","s, Operation::Comma },
            { "."s, Operation::Dot },
            { "->"s, Operation::Lambda },
            { "||"s, Operation::Or },
            { "&&"s, Operation::And },
            { "%"s, Operation::Mod },
            { "/"s, Operation::Divide },
            { "//"s, Operation::FloorDivision }
        };

        auto strip_view(string_view sv) {
            sv.remove_prefix(min(sv.find_first_not_of(" "), sv.size()));
            sv.remove_suffix(sv.size() - sv.find_last_not_of(" ") - 1);
            return sv;
        }

        // function is either one of the built-in structures, or constructor/function defined in source file
        void addFunctionNode(const string_view name, const int priority, Tree* const currentTree) {
            if (name == "if") currentTree->addNode(priority, Operation::If);
            else if (name == "while") currentTree->addNode(priority, Operation::While);
            else if (name == "return") currentTree->addNode(priority, Operation::Return);
            else if (name == "print") currentTree->addNode(priority, Operation::Print);
            else if (name == "int") currentTree->addNode(priority, Operation::ConvertToInt);
            else if (name == "double") currentTree->addNode(priority, Operation::ConvertToDouble);
            else if (name == "sqrt") currentTree->addNode(priority, Operation::SquareRoot);
            else if (name == "char")currentTree->addNode(priority, Operation::Char);
            else if (name == "new_set")currentTree->addNode(priority, Operation::Set);
            else if (name == "dict") currentTree->addNode(priority, Operation::Dict);
            else currentTree->addNode(priority, (isupper(name[0]) ? Operation::Constructor : Operation::Function), name);
        }

        void openBlock(const string_view sv, stack<Block*>& blocks, unique_ptr<Tree> currentTree) {
            auto blockReceiver = (currentTree
                                  ? currentTree->getTopNode()
                                  : blocks.top()->lastLine()->getCurrentNode());

            auto block = make_unique<Block>();
            auto blockPointer = block.get();

            if (Util::Regex::isFunctionDefinition(sv)) {
                auto op = blockReceiver->lastOperand();
                // if function has max 1 argument (otherwise op would be unique_ptr<Node>)
                if (holds_alternative<Object*>(*op)) {
                    if (auto ref = dynamic_cast<Reference*>(get<Object*>(*op))) {
                        auto argumentName = ref->getName();
                        if (sv.substr(0, 11) == "constructor") {
                            auto name = strip_view(string_view(&*(cbegin(sv) + 11), sv.find_first_of('(') - 11));
                            if (!Util::Regex::isFunctionDefinition(argumentName))
                                Functions::createConstructor(name, argumentName, move(block));
                            else Functions::createConstructor(name, move(block));
                        }
                        else {
                            auto functionName = strip_view(string_view(&*(cbegin(sv) + 9), sv.find_first_of('(') - 9));
                            // if funcion is not a method
                            if (auto pos = functionName.find("."); pos == string_view::npos) {
                                if (!Util::Regex::isFunctionDefinition(argumentName))
                                    Functions::createGlobal(functionName, argumentName, move(block));
                                else Functions::createGlobal(functionName, move(block));
                            }
                            else {
                                auto className = functionName.substr(0, pos);
                                auto methodName = functionName.substr(pos + 1);
                                if (!Util::Regex::isFunctionDefinition(argumentName))
                                    Functions::create(className, methodName, argumentName, move(block));
                                else Functions::create(className, methodName, move(block));
                            }
                        }
                    }
                }
                else {
                    // op is CommaNode, convert its operands to arg names
                    auto variableNames = Util::Transform::toArgumentNames(get<unique_ptr<Node>>(*op)->front(),
                                                                          get<unique_ptr<Node>>(*op)->back());
                    if (isObject(variableNames)) {
                        if (sv.substr(0, 11) == "constructor") {
                            auto name = strip_view(string_view(&*(cbegin(sv) + 11), sv.find_first_of('(') - 11));
                            Functions::createConstructor(name, get<vector<string_view>>(variableNames), move(block));
                        }
                        else {
                            auto functionName = strip_view(string_view(&*(cbegin(sv) + 9), sv.find_first_of('(') - 9));
                            if (auto pos = functionName.find("."); pos == string_view::npos) {
                                Functions::createGlobal(functionName, get<vector<string_view>>(variableNames), move(block));
                            }
                            else {
                                auto className = functionName.substr(0, pos);
                                auto methodName = functionName.substr(pos + 1);
                                Functions::create(className, methodName, get<vector<string_view>>(variableNames), move(block));
                            }
                        }
                    }
                }
            }
            else { // If, While etc.
                blockReceiver->add(move(block));
                if (currentTree) {
                    currentTree->setCurrentNode(blockReceiver);
                    blocks.top()->addLine(move(currentTree));
                }
            }
            blocks.push(blockPointer);
        }

        void parseLine(const string_view line, stack<Block*>& blocks) {
            auto currentTree = unique_ptr<Tree>();
            auto lastMatchIndex = optional<int> {};
            auto bracketsOpen = 0;
            auto match_end = regex_iterator<string_view::const_iterator>();
            auto readingString = false;
            ptrdiff_t stringStartPosition;
            for (auto i = Util::Regex::operandIterator(line); i != match_end; ++i) {
                // for each operator found
                auto position = i->position();
                auto length = i->length();
                auto operator_view = string_view(&*(cbegin(line) + position), length);

                // if it's an apostrophe, it marks either begin or end of a string
                if (operator_view == "'") {
                    if (readingString == false) {
                        readingString = true;
                        stringStartPosition = position;
                    }
                    else {
                        auto xd = string_view(&*(cbegin(line) + stringStartPosition), position - stringStartPosition);
                        currentTree->add(xd);
                        readingString = false;
                    }
                    lastMatchIndex = position + length - 1;
                    continue;
                }
                // opening new block, for function definition, If statement etc.
                if (operator_view == "{") {
                    openBlock(strip_view(line), blocks, move(currentTree));
                    return;
                }

                if (operator_view == "}") {
                    blocks.pop();
                    return;
                }

                auto offset = lastMatchIndex.value_or(-1) + 1;
                auto operand_view = strip_view(string_view(&*(cbegin(line) + offset), position - offset));

                // ignore dots in Double literals, or method definitions
                if (operator_view == "." && !operand_view.empty() && (all_of(cbegin(operand_view), cend(operand_view),
                                                                             [](const char c) -> bool { return isdigit(c); })
                                                                      || Util::Regex::isMethodDefinition(operand_view))) {
                    continue;
                }

                if (operator_view == "(") {
                    if (!operand_view.empty()) {
                        if (!currentTree) currentTree = make_unique<Tree>();
                        addFunctionNode(operand_view, priorityCount * (1 + bracketsOpen) - 1, currentTree.get());
                    }
                    bracketsOpen++;
                }
                else if (operator_view == ")") {
                    if (!operand_view.empty()) {
                        if (!currentTree) currentTree = make_unique<Tree>();
                        currentTree->add(operand_view);
                    }
                    bracketsOpen--;
                }
                else {
                    auto basePriority = bracketsOpen * priorityCount;
                    if (!currentTree) currentTree = make_unique<Tree>();
                    if (Util::Regex::isAugmentedAssignment(operator_view)) { // x += 2 is converted to x = x + 2
                        auto priority = operationPriority[Operation::Assign] + basePriority;
                        currentTree->addNode(priority, Operation::Assign, operand_view);
                        operator_view.remove_suffix(1);
                        currentTree->addNode(priority, symbolToOperation.find(operator_view)->second, operand_view);
                    }
                    else {
                        auto operation = symbolToOperation.find(operator_view)->second;
                        currentTree->addNode(operationPriority[operation] + basePriority, operation, operand_view);
                    }
                }
                lastMatchIndex = position + length - 1;
            }

            // finalize line
            if (currentTree && lastMatchIndex != line.length()) {
                if (auto startOffset = line.find_first_not_of(") ", lastMatchIndex.value_or(0) + 1); startOffset != string::npos) {
                    if (auto endOffset = line.find_last_not_of("{ "); endOffset >= startOffset) {
                        currentTree->add(string_view(&*(cbegin(line) + startOffset), endOffset - startOffset + 1));
                    }
                }
            }

            if (currentTree) blocks.top()->addLine(move(currentTree));
        }
    }

    int getPriorityCount() {
        return priorityCount;
    }

    bool isAssignment(int priority) {
        return priority % priorityCount == operationPriority[Operation::Assign];
    }

    unique_ptr<Block> parseFile(const std::string& filename) {
        auto resultBlock = make_unique<Block>();
        auto blockStack = stack<Block*> {};
        blockStack.emplace(resultBlock.get());

        auto f = ifstream { filename };
        auto s = string {};
        while (getline(f, s)) {
            parseLine(s, blockStack);
        }
        return resultBlock;
    }

    unique_ptr<Block> testLine(const std::string& line) {
        auto resultBlock = make_unique<Block>();
        auto blockStack = stack<Block*> {};
        blockStack.emplace(resultBlock.get());
        parseLine(line, blockStack);
        return resultBlock;
    }

}