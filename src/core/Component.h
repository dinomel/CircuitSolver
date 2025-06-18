#pragma once
#include <td/String.h>

class Component
{
public:
    int id;
    td::String name;
    td::String symbol;

    static int nextID;

    static int getNextID()
    {
        nextID++;
        return nextID;
    }

    Component(
        const td::String &name,
        const td::String &symbol)
        : id(getNextID()),
          name(name),
          symbol(symbol) {}

    virtual ~Component() = default;
};
