#pragma once

namespace ReferenceStack {
    Object* get(const std::string_view sv);

    void set(const std::string_view sv, Object* o);

    void push();

    void pop();

    void pop(Object* o);
}