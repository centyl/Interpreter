#include <map>
#include <stack>

#include "Object.h"

namespace ReferenceStack {

    using namespace std;

    namespace {
        stack<map<string, Object*, less<>>> v { };
    }

    Object* get(const string_view sv) {
        auto it = v.top().find(sv);
        return (it != end(v.top()) ? it->second : nullptr);
    }

    void set(const string_view sv, Object* o) {
        v.top()[string(sv)] = o;
        o->incrementReferenceCounter();
    }

    void push() {
        v.push({});
    }

    void pop() {
        for (auto e : v.top())
            e.second->decrementAndDestroy();

        v.pop();
    }

    void pop(Object* o) {
        for (auto e : v.top()) {
            e.second->decrementReferenceCounter();
            if (e.second != o) e.second->destructIfNotReferenced();
        }
        v.pop();
    }
}