#pragma once

#include <map>

#include "Object.h"

class Dict :
    public Object {
    std::map<std::string, Object*> objects;
    public:

    virtual ObjectOrError runMethod(const std::string_view s, const std::vector<Object*>& arguments) override;

    std::optional<std::string> repr() override;

};

