#pragma once
#include <td/String.h>

class Component
{
public:
    td::String id;
    td::String name;
    td::String symbol;
    td::String image;

    Component(
        const td::String &name,
        const td::String &symbol,
        const td::String &image = "",
        const td::String &id = "")
        : id(id),
          name(name),
          symbol(symbol),
          image(image) {}

    virtual ~Component() = default;
};
