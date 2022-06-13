#pragma once

#include "Object.h"

class NullObject : public Object {

    std::optional<std::string> repr() override;

};