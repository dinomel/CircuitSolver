//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/SymbolPopoverView.h>
#include <gui/Symbol.h>

class SymbolsPopover : public gui::SymbolPopoverView
{
    gui::Symbol _pointer;
    gui::Symbol _wire;
    gui::Symbol _resistor;
    gui::Symbol _capacitor;
    gui::Symbol _inductor;
    gui::Symbol _dcVoltage;
    gui::Symbol _acVoltage;
    gui::Symbol _current;

public:
    SymbolsPopover()
        : SymbolPopoverView(gui::PopoverView::Type::SymbolOnly, 4, 4), _pointer(":pointer"), _wire(":wire"), _resistor(":resistor"), _capacitor(":capacitor"), _inductor(":inductor"), _dcVoltage(":inductor"), _acVoltage(":inductor"), _current(":inductor")
    {
        gui::Symbol *nullSymbol = nullptr;
        addItem(&_pointer, nullSymbol, tr("pointer"), tr("pointerTT"), false, 0);
        addItem(&_wire, nullSymbol, tr("rndRct"), tr("rndRctTT"), true, 0);
        addItem(&_resistor, nullSymbol, tr("rndRct"), tr("rndRctTT"), true, 0);
        addItem(&_capacitor, nullSymbol, tr("rndRct"), tr("rndRctTT"), true, 0);
        addItem(&_inductor, nullSymbol, tr("rndRct"), tr("rndRctTT"), true, 0);
        addItem(&_dcVoltage, nullSymbol, tr("rndRct"), tr("rndRctTT"), true, 0);
        addItem(&_acVoltage, nullSymbol, tr("rndRct"), tr("rndRctTT"), true, 0);
        addItem(&_current, nullSymbol, tr("rndRct"), tr("rndRctTT"), true, 0);
        //        setItem(itemPos++, &_img, tr("Save"), tr("SaveTT"), 3.0f, 3.0f);

        styleOnParent(false); // remove this line if you want to see color symbols on toolbar
    }
};
