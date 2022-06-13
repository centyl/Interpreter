#pragma once

#include "Object.h"

class String : public Object {
    std::string value;
    public:
    String(const std::string_view value);

    virtual void erase() override;
    ObjectOrError execute(Operation operation, Object* other) override;

    std::optional<std::string> repr() override;

    virtual ObjectOrError runMethod(const std::string_view s, const std::vector<Object*>& arguments) override;
};