#pragma once

#include "Block.h"

using Function = std::pair<std::vector<std::string>, std::unique_ptr<Block>>;

namespace Functions {
    void createGlobal(const std::string_view name, const std::vector<std::string_view>& argumentNames, std::unique_ptr<Block> body);

    void createGlobal(const std::string_view name, std::string_view argumentName, std::unique_ptr<Block> body);

    void createGlobal(const std::string_view name, std::unique_ptr<Block> body);

    void create(const std::string_view className, const std::string_view methodName, const std::vector<std::string_view>& argumentNames, std::unique_ptr<Block> body);

    void create(const std::string_view className, const std::string_view methodName, std::string_view argumentName, std::unique_ptr<Block> body);

    void create(const std::string_view className, const std::string_view methodName, std::unique_ptr<Block> body);

    void createConstructor(const std::string_view className, const std::vector<std::string_view>& argumentNames, std::unique_ptr<Block> body);

    void createConstructor(const std::string_view className, std::string_view argumentName, std::unique_ptr<Block> body);

    void createConstructor(const std::string_view className, std::unique_ptr<Block> body);

    Function* findGlobal(const std::string_view name, int argumentCount);

    Function* find(const std::string_view className, const std::string_view methodName, int argumentCount);

    Function* findConstructor(const std::string_view className, int argumentCout);

};
