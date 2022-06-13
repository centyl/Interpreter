#pragma once

#include "Object.h"

class Array : public Object {
    std::vector<Object*> objects;

    public:

    Array(std::vector<Object*>&& objects);

    virtual std::optional<std::string> repr() override;

    virtual ObjectOrError execute(Operation operation, Object* other) override;

    virtual void erase() override;

    virtual std::optional<bool> booleanValue() override;

    virtual ObjectOrError runMethod(const std::string_view s, const std::vector<Object*>& arguments) override;

    virtual ObjectOrError runMethod(const std::string_view s, const std::string_view argumentName, Node* n) override;
};

