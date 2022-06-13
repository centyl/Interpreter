#pragma once

#include "Object.h"

class Boolean : public Object {
    bool value;

    public:
    Boolean(bool b);

    virtual std::optional<std::string> repr() override;

    virtual std::optional<int> intValue() override;

    virtual std::optional<bool> booleanValue() override;

    virtual ObjectOrError execute(Operation operation, Object* other) override;
};

