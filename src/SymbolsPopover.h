#pragma once
#include <gui/SymbolPopoverView.h>

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
        addItem(&_ground, nullSymbol, tr("ground"), tr("groundTT"), true, 0);

        styleOnParent(false);
    }
};
