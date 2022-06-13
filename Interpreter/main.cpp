#include <iostream>
#include <string_view>

#include "Parser.h"
#include "ReferenceStack.h"

using namespace std;

int main(int argc, char* argv[]) {
    auto res = Parser::parseFile((argc == 1 ? "source.txt" : argv[1]));
    if (res) {
        ReferenceStack::push();
        auto result = res->calculate();
        if (isError(result)) {
            cout << get<unique_ptr<Error>>(result)->getErrorMessage() << endl;
        }
    }
    return 0;
}