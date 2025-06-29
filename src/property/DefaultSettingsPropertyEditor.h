#pragma once
#include <gui/ViewSwitcher.h>
#include <gui/PropertyEditor.h>
#include "DefaultSettings.h"

extern DefaultSettings g_defaultSettings;

class DefaultSettingsPropertyEditor : public gui::PropertyEditor
{
private:
protected:
    gui::Properties _properties;

public:
    DefaultSettingsPropertyEditor()
        : gui::PropertyEditor(gui::tr("Defaults"), true)
    {
        g_defaultSettings.createPropertiesForEditor(_properties);
        setProperties(&_properties);
    }
};
