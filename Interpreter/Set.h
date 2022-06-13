#pragma once

#include <set>

#include "Object.h"

class Set : public Object {
    std::set<Object*> value;
    public:

    Set(std::vector<Object*>&& objects);

    std::optional<std::string> repr() override;

    virtual ObjectOrError runMethod(const std::string_view s, const std::vector<Object*>& arguments) override;

};

