#pragma once
#include <td/String.h>

class Component
{
public:
    int id;
    td::String name;
    td::String symbol;
    td::String image;

    static int nextID;

    static int getNextID()
    {
        nextID++;
        return nextID;
    }

    Component(
        const td::String &name,
        const td::String &symbol,
        const td::String &image = "")
        : id(getNextID()),
          name(name),
          symbol(symbol),
          image(image) {}

    virtual ~Component() = default;
};
