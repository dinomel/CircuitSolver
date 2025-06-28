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
    gui::PropertyEditor _currentPropEditor;
    gui::PropertyEditor _groundPropEditor;

public:
    PropertyEditors()
        : gui::PropertyEditorSwitcher(9), _wirePropEditor(gui::tr("wire"), IGridComponent::getProperties(IGridComponent::Type::Wire)), _resistorPropEditor(gui::tr("resistor"), IGridComponent::getProperties(IGridComponent::Type::Resistor)), _capacitorPropEditor(gui::tr("capacitor"), IGridComponent::getProperties(IGridComponent::Type::Capacitor)), _inductorPropEditor(gui::tr("inductor"), IGridComponent::getProperties(IGridComponent::Type::Inductor)), _dcVoltagePropEditor(gui::tr("dcVoltage"), IGridComponent::getProperties(IGridComponent::Type::DCVoltageSource)), _acVoltagePropEditor(gui::tr("acVoltage"), IGridComponent::getProperties(IGridComponent::Type::ACVoltageSource)), _currentPropEditor(gui::tr("currentSource"), IGridComponent::getProperties(IGridComponent::Type::CurrentSource)), _groundPropEditor("Ground", IGridComponent::getProperties(IGridComponent::Type::Ground))
    {
        addView(&_defaultPropEditor);
        addView(&_wirePropEditor);
        addView(&_resistorPropEditor);
        addView(&_capacitorPropEditor);
        addView(&_inductorPropEditor);
        addView(&_dcVoltagePropEditor);
        addView(&_acVoltagePropEditor);
        addView(&_currentPropEditor);
        addView(&_groundPropEditor);
        setCurrentEditor(0, &g_defaultSettings, true);
    }
};
