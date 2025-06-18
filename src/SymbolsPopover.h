//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/SymbolPopoverView.h>
// #include <gui/Symbol.h>

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
    gui::Symbol _ground;

public:
    SymbolsPopover()
        : SymbolPopoverView(gui::PopoverView::Type::SymbolOnly, 4, 4), _pointer(":pointer"), _wire(":wire"), _resistor(":resistor"), _capacitor(":capacitor"), _inductor(":inductor"), _dcVoltage(":dc_voltage_source"), _acVoltage(":ac_voltage_source"), _current(":current_source"), _ground(":ground")
    {
        gui::Symbol *nullSymbol = nullptr;
        addItem(&_pointer, nullSymbol, tr("pointer"), tr("pointerTT"), false, 0);
        addItem(&_wire, nullSymbol, tr("wire"), tr("wireTT"), true, 0);
        addItem(&_resistor, nullSymbol, tr("resistor"), tr("resistorTT"), true, 0);
        addItem(&_capacitor, nullSymbol, tr("capacitor"), tr("capacitorTT"), true, 0);
        addItem(&_inductor, nullSymbol, tr("inductor"), tr("inductorTT"), true, 0);
        addItem(&_dcVoltage, nullSymbol, tr("dcVoltage"), tr("dcVoltageTT"), true, 0);
        addItem(&_acVoltage, nullSymbol, tr("acVoltage"), tr("acVoltageTT"), true, 0);
        addItem(&_current, nullSymbol, tr("currentSource"), tr("currentTT"), true, 0);
        addItem(&_ground, nullSymbol, tr("rndRct"), tr("rndRctTT"), true, 0);
        //        setItem(itemPos++, &_img, tr("Save"), tr("SaveTT"), 3.0f, 3.0f);

        styleOnParent(false); // remove this line if you want to see color symbols on toolbar
    }
};
