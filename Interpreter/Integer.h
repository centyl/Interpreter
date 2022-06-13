#pragma once

#include "Object.h"

class Integer : public Object {
    int value;

    public:
    Integer(int v);

    std::optional<std::string> repr() override;

    ObjectOrError execute(Operation operation, Object* other) override;

    std::optional<int> intValue() override;

    std::optional<double> doubleValue() override;

    virtual std::optional<bool> booleanValue() override;

    virtual void erase() override;
};