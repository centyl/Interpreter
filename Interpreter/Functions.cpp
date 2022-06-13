#include <map>

#include "Functions.h"

namespace Functions {

    using namespace std;

    namespace {
        // key is function name and number of arguments, value is argument names and function body
        using FunctionMap = map<pair<string, int>, Function>;

        // key is "" for global functions, and class name for methods
        map<string, FunctionMap, less<>> namespaces;

        // for each string: class, stores constructor function definitions mapped by number of arguments expected
        map<string, map<int, Function>, less<>> constructors;
    }

    void createGlobal(const string_view name, const vector<string_view>& argumentNames, unique_ptr<Block> body) {
        create("", name, argumentNames, move(body));
    }

    void createGlobal(const std::string_view name, std::string_view argumentName, std::unique_ptr<Block> body) {
        create("", name, vector<string_view>{ argumentName }, move(body));
    }

    void createGlobal(const std::string_view name, std::unique_ptr<Block> body) {
        create("", name, vector<string_view>{ }, move(body));
    }

    void create(const string_view className, const string_view methodName, const vector<string_view>& argumentNames, unique_ptr<Block> body) {
        if (namespaces.find(className) == namespaces.end()) namespaces.emplace(className, FunctionMap {});
        namespaces[string(className)][{methodName, argumentNames.size()}] = { vector<string>{cbegin(argumentNames), cend(argumentNames)}, move(body) };
    }

    void create(const std::string_view className, const std::string_view methodName, std::string_view argumentName, std::unique_ptr<Block> body) {
        create(className, methodName, vector<string_view>{argumentName}, move(body));
    }

    void create(const std::string_view className, const std::string_view methodName, std::unique_ptr<Block> body) {
        create(className, methodName, vector<string_view>{}, move(body));
    }

    void createConstructor(const std::string_view className, const std::vector<std::string_view>& argumentNames, std::unique_ptr<Block> body) {
        if (constructors.find(className) == constructors.end()) constructors.emplace(className, map<int, Function>{});
        constructors[string(className)][argumentNames.size()] = { vector<string>{cbegin(argumentNames), cend(argumentNames)}, move(body) };
    }

    void createConstructor(const std::string_view className, std::string_view argumentName, std::unique_ptr<Block> body) {
        createConstructor(className, vector<string_view>{argumentName}, move(body));
    }

    void createConstructor(const std::string_view className, std::unique_ptr<Block> body) {
        createConstructor(className, vector<string_view>{}, move(body));
    }

    Function* findGlobal(const string_view s, int argumentCount) {
        return find("", s, argumentCount);
    }

    Function* find(const string_view className, const string_view methodName, int argumentCount) {
        auto methods = namespaces.find(className);
        if (methods != namespaces.end()) {
            auto function = methods->second.find(make_pair(string(methodName), argumentCount));
            return (function != methods->second.end() ? addressof(function->second) : nullptr);
        }
        return nullptr;
    }

    Function* findConstructor(const std::string_view className, int argumentCount) {
        auto cons = constructors.find(className);
        if (cons != constructors.end()) {
            auto function = cons->second.find(argumentCount);
            return (function != cons->second.end() ? addressof(function->second) : nullptr);
        }
        return nullptr;
    }

}