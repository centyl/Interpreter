#pragma once

#include "Object.h"

class Double : public Object {
    double value;

    public:
    Double(double d) : value(d) { }

    std::optional<std::string> repr() override;

    ObjectOrError execute(Operation operation, Object* other) override;

    std::optional<int> intValue() override;

    std::optional<double> doubleValue() override;

    virtual void erase() override;
};

