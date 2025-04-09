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

public:
    PropertyEditors()
        : gui::PropertyEditorSwitcher(4), _resistorPropEditor(gui::tr("Resistor"), IGridComponent::getProperties(IGridComponent::Type::Resistor)), _capacitorPropEditor(gui::tr("Capacitor"), IGridComponent::getProperties(IGridComponent::Type::Capacitor)), _inductorPropEditor(gui::tr("Inductor"), IGridComponent::getProperties(IGridComponent::Type::Inductor))
    {
        //        setMargins(0, 0, 15, 5);
        addView(&_defaultPropEditor);
        addView(&_resistorPropEditor);
        addView(&_capacitorPropEditor);
        addView(&_inductorPropEditor);
        setCurrentEditor(0, &g_defaultSettings, true);
    }
};
