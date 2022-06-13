#pragma once

#include "Object.h"

class Reference : public Object {
    std::string name;

    public:
    std::string_view getName();

    Reference(const std::string_view name);

    std::optional<int> intValue() override;

    std::optional<double> doubleValue() override;

    ObjectOrError assign(Object* other);

    ObjectOrError execute(Operation operation, Object* other) override;

    std::optional<std::string> repr() override;

    virtual ObjectOrError runMethod(const std::string_view s, const std::vector<Object*>& arguments) override;

    virtual ObjectOrError runMethod(const std::string_view s, const std::string_view argumentName, Node* n);

    virtual ObjectOrError unref() override;

    virtual std::optional<bool> booleanValue() override;

};