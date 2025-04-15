//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/PropertyEditorSwitcher.h>
#include "DefaultSettingsPropertyEditor.h"
#include "../model/IGridComponent.h"

class PropertyEditors : public gui::PropertyEditorSwitcher
{
private:
protected:
    DefaultSettingsPropertyEditor _defaultPropEditor;
    gui::PropertyEditor _resistorPropEditor;
    gui::PropertyEditor _capacitorPropEditor;
    gui::PropertyEditor _inductorPropEditor;
    gui::PropertyEditor _wirePropEditor;
    gui::PropertyEditor _dcVoltagePropEditor;
    gui::PropertyEditor _acVoltagePropEditor;

public:
    PropertyEditors()
        : gui::PropertyEditorSwitcher(4), _wirePropEditor("Wire", IGridComponent::getProperties(IGridComponent::Type::Wire)), _resistorPropEditor("Resistor", IGridComponent::getProperties(IGridComponent::Type::Resistor)), _capacitorPropEditor("Capacitor", IGridComponent::getProperties(IGridComponent::Type::Capacitor)), _inductorPropEditor("Inductor", IGridComponent::getProperties(IGridComponent::Type::Inductor)), _dcVoltagePropEditor("DC Voltage", IGridComponent::getProperties(IGridComponent::Type::DCVoltageSource)), _acVoltagePropEditor("AC Voltage", IGridComponent::getProperties(IGridComponent::Type::ACVoltageSource))
    {
        //        setMargins(0, 0, 15, 5);
        addView(&_defaultPropEditor);
        addView(&_wirePropEditor);
        addView(&_resistorPropEditor);
        addView(&_capacitorPropEditor);
        addView(&_inductorPropEditor);
        addView(&_dcVoltagePropEditor);
        addView(&_acVoltagePropEditor);
        setCurrentEditor(0, &g_defaultSettings, true);
    }
};
