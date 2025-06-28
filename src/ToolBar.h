#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
#include "SymbolsPopover.h"

class ToolBar : public gui::ToolBar
{
protected:
    gui::Image _imgSave;
    SymbolsPopover _symbolsPopover;
    gui::Image _settings;

public:
    ToolBar()
        : gui::ToolBar("mainToolBar", 6, 6), _imgSave(":imgSave"), _settings(":settings")
    {
        addItem(tr("settings"), &_settings, tr("settingsTT"), 10, 0, 0, 10);

        addSpaceItem();

        addItem(tr("Export"), &_imgSave, tr("ExportTT"), 20, 0, 0, 20);

        addSpaceItem();

        addItem(&_symbolsPopover, 1024);
    }

    SymbolsPopover *getSymbolsPopover()
    {
        return &_symbolsPopover;
    }
};
