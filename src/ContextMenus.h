#pragma once
#include <gui/ContextMenus.h>

class ContextMenu : public gui::ContextMenu
{
private:
    gui::SubMenu _cmi;

protected:
public:
    ContextMenu()
        : _cmi(100, "cm1", 4)
    {
        auto &items = _cmi.getItems();
        items[0].initAsActionItem(tr("SetFront"), 10); // move component to front
        items[1].initAsActionItem(tr("SetBack"), 20);  // move component to background
        items[2].initAsSeparator();
        items[3].initAsActionItem(tr("Delete"), 30); // delete component
        setItem(&_cmi);
    }
};

class ContextMenus : public gui::ContextMenus
{
protected:
    ContextMenu _cm1;

public:
    ContextMenus()
        : gui::ContextMenus(1) // one menu
    {
        setItem(0, &_cm1);
    }
};
