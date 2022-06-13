#include <algorithm>

#include "Objects.h"
#include "RegexUtil.h"
#include "TransformUtil.h"
#include "Reference.h"
#include "String.h"

using namespace std;

namespace Util {
    namespace Transform {

        namespace {
            auto containsNoDuplicates(const vector<string_view>& v) {
                auto temp = vector<string_view> { cbegin(v), cend(v) };
                sort(begin(temp), end(temp));
                return unique(begin(temp), end(temp)) == end(temp);
            }
        }

        vector<ObjectOrError>::iterator findError(vector<ObjectOrError>::iterator begin,
                                                  vector<ObjectOrError>::iterator end) {
            return find_if(begin, end, [](auto&& o) -> bool { return isError(o); });
        }

        vector<ObjectOrError> toUnreferenced(vector<Object*>::const_iterator begin,
                                             vector<Object*>::const_iterator end) {
            auto result = vector<ObjectOrError> {};
            transform(begin, end, back_inserter(result), [](Object* o) -> ObjectOrError { return o->unref(); });
            return result;
        }

        vector<Object*> toObject(vector<ObjectOrError>::const_iterator begin,
                                 vector<ObjectOrError>::const_iterator end) {
            auto result = vector<Object*> {};
            transform(begin, end, back_inserter(result), [](auto&& o) -> Object* { return get<Object*>(o); });
            return result;
        }

        vector<ObjectOrError> toObjectOrError(vector<Operand>::const_iterator begin,
                                              vector<Operand>::const_iterator end) {
            auto result = vector<ObjectOrError> {};
            transform(begin, end, back_inserter(result), [](auto&& o) -> ObjectOrError { return Util::Transform::toObject(o); });
            return result;
        }

        ObjectsOrError toObjects(vector<Operand>::const_iterator beg, vector<Operand>::const_iterator las) {
            auto optionalObjects = toObjectOrError(beg, las);
            auto it = findError(begin(optionalObjects), end(optionalObjects));
            return (it == end(optionalObjects)
                    ? toObject(cbegin(optionalObjects), cend(optionalObjects))
                    : (ObjectsOrError) move(get<unique_ptr<Error>>(*it)));
        }

        ObjectsOrError toUnreferencedObjects(vector<Object*>::const_iterator it_begin,
                                             vector<Object*>::const_iterator it_end) {
            auto unrefd = toUnreferenced(it_begin, it_end);
            auto it = findError(begin(unrefd), end(unrefd));
            return (it == end(unrefd)
                    ? toObject(cbegin(unrefd), cend(unrefd))
                    : (ObjectsOrError) move(get<unique_ptr<Error>>(*it)));
        }

        ObjectsOrError toUnreferencedObjects(vector<Operand>::const_iterator it_begin,
                                             vector<Operand>::const_iterator it_end) {
            auto result = toObjects(it_begin, it_end);
            if (isObject(result)) {
                auto objects = get<vector<Object*>>(result);
                return toUnreferencedObjects(cbegin(objects), cend(objects));
            }
            return result;
        }

        OptionalError<std::vector<std::string_view>> toArgumentNames(std::vector<Operand>::const_iterator it_begin, std::vector<Operand>::const_iterator it_end) {
            if (all_of(it_begin, it_end, [](const auto& o) -> bool {return holds_alternative<Object*>(o); })) {
                auto references = vector<Reference*> {};
                transform(it_begin, it_end, back_inserter(references),
                          [](const auto& o) -> Reference* { return dynamic_cast<Reference*>(get<Object*>(o)); });
                if (all_of(cbegin(references), cend(references), [](const auto* r) -> bool { return r; })) {
                    auto variableNames = vector<string_view> {};
                    transform(cbegin(references), cend(references), back_inserter(variableNames),
                              [](const auto& o) -> string_view { return o->getName(); });
                    if (containsNoDuplicates(variableNames))
                        return variableNames;
                }
            }
            return make_unique<Error>(ErrorType::UnknownError);
        }

        Object* toObject(std::string_view s) {
            if (all_of(cbegin(s), cend(s), [](char c) { return isdigit(c); }))
                return Objects::getOrCreate(stoi(string(s)));

            if (Util::Regex::isDouble(s))
                return Objects::getOrCreate(stod(string(s)));

            if (s[0] == '\'') {
                s.remove_prefix(1);
                return new String(s);
            }

            return Objects::getOrCreate(s);
        }

        ObjectOrError toObject(const Operand& operand) {
            if (holds_alternative<unique_ptr<Node>>(operand)) return get<unique_ptr<Node>>(operand)->calculate();
            if (holds_alternative<Object*>(operand)) return get<Object*>(operand);
            return make_unique<Error>(ErrorType::UnknownError);
        }

    }
}