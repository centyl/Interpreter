#pragma once

#include "Integer.h"
#include "Reference.h"
#include "Boolean.h"
#include "Double.h"
#include "NullObject.h"

namespace Objects {
    void clear();

    Integer* getOrCreate(int v);

    Reference* getOrCreate(const std::string_view s);

    Double* getOrCreate(double d);

    Boolean* get(bool b);

    NullObject* getNullObject();

    void remove(int v);
    void remove(double d);

    void print();
};