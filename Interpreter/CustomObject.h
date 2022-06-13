#pragma once

#include <map>

#include "Object.h"

class CustomObject : public Object {
    std::string className;
    std::map<std::string, Object*, std::less<>> fields;

    public:
    CustomObject(const std::string_view className);

    std::optional<std::string> repr() override;

    virtual void erase() override;

    virtual ObjectOrError runMethod(const std::string_view s, const std::vector<Object*>& arguments) override;

};

