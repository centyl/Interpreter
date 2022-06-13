#include "FunctionUtil.h"
#include "ReferenceStack.h"
#include "TransformUtil.h"
#include "CommaNode.h"

namespace Util {

    namespace Function {

        using namespace std;

        int getArgumentCount(std::vector<Operand>::const_iterator begin, std::vector<Operand>::const_iterator end) {
            if (distance(begin, end) == 1) return 0;
            if (holds_alternative<Object*>(begin[1])) return 1;
            auto node = get<unique_ptr<Node>>(begin[1]).get();
            return (dynamic_cast<CommaNode*>(node) ? node->operandCount() : 1);
        }

        void pushArguments(const vector<string>& argumentNames, const vector<Object*>& arguments) {
            ReferenceStack::push();
            for (auto i = 0; i < argumentNames.size(); i++) {
                ReferenceStack::set(argumentNames[i], arguments[i]);
            }
        }

        ObjectsOrError getArguments(std::vector<Operand>::const_iterator begin_it, std::vector<Operand>::const_iterator end_it) {
            if (distance(begin_it, end_it) > 1) {
                if (holds_alternative<unique_ptr<Node>>(begin_it[1])) {
                    auto n = get<unique_ptr<Node>>(begin_it[1]).get();

                    if (dynamic_cast<CommaNode*>(n))
                        return Util::Transform::toUnreferencedObjects(n->front(), n->back());

                    auto res = n->calculate();
                    return (isObject(res)
                            ? vector<Object*> { get<Object*>(res) }
                    : (ObjectsOrError) move(get<unique_ptr<Error>>(res)));
                }
                auto un = get<Object*>(begin_it[1])->unref();

                return (isObject(un)
                        ? vector<Object*> { get<Object*>(un) }
                : (ObjectsOrError) move(get<unique_ptr<Error>>(un)));

            }
            return vector<Object*> {};
        }
    }
}