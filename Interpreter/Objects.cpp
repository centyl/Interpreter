#include <map>

#include "Integer.h"
#include "Reference.h"
#include "Double.h"
#include "NullObject.h"
#include "Boolean.h"
#include "Objects.h"

using namespace std;

namespace Objects {
    namespace {
        map<int, unique_ptr<Integer>> integerMap;
        map<string, unique_ptr<Reference>, less<>> referenceMap;
        map<double, unique_ptr<Double>> doubleMap;

        Boolean trueValue { true };
        Boolean falseValue { false };

        NullObject nullObject {};
    }

    void clear() {
        integerMap.clear();
        referenceMap.clear();
    }

    Integer* getOrCreate(int v) {
        auto o = integerMap.find(v);
        return (o != integerMap.end()
                ? o->second.get()
                : integerMap.emplace(v, make_unique<Integer>(v)).first->second.get());
    }

    Reference* getOrCreate(const std::string_view s) {
        auto o = referenceMap.find(s);
        return (o != referenceMap.end()
                ? o->second.get()
                : referenceMap.emplace(s, make_unique<Reference>(s)).first->second.get());
    }

    Double* getOrCreate(double d) {
        auto o = doubleMap.find(d);
        return (o != doubleMap.end()
                ? o->second.get()
                : doubleMap.emplace(d, make_unique<Double>(d)).first->second.get());
    }

    Boolean* Objects::get(bool b) {
        return addressof(b ? trueValue : falseValue);
    }

    NullObject* getNullObject() {
        return addressof(nullObject);
    }

    void Objects::remove(int v) {
        integerMap.erase(v);
    }

    void Objects::remove(double d) {
        doubleMap.erase(d);
    }

    void Objects::print() {

    }

}