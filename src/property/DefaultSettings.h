#pragma once
#include <gui/Shape.h>
#include <gui/IProperty.h>
#include <gui/Properties.h>
#include "../model/IGridComponent.h"

class DefaultSettings : public gui::IProperty
{
    enum class PropID : td::UINT4
    {
        Frequency,
        AutoSolve,
        ComponentColor,
        SelectedComponentColor
    };

protected:
    float _frequency = 50;
    bool _autoSolve = true;
    td::ColorID _componentColor = td::ColorID::Yellow;
    td::ColorID _selectedComponentColor = td::ColorID::SpringGreen;

protected:
    inline void setValueByKey(gui::PropertyValues &propValues, PropID propID, const td::Variant &val) const
    {
        propValues.setValueByKey((td::UINT4)propID, val);
    }

    inline const td::Variant &getValueByKey(gui::PropertyValues &propValues, PropID propID)
    {
        return propValues.getValueByKey((td::UINT4)propID);
    }

public:
    DefaultSettings()
    {
    }

    float getFrequency() const
    {
        return _frequency;
    }

    void setFrequency(float frequency)
    {
        _frequency = frequency;
    }

    void setAutoSolve(int autoSolve)
    {
        _autoSolve = (bool)autoSolve;
    }

    float getAutoSolve() const
    {
        return _autoSolve;
    }

    td::ColorID getComponentColor() const
    {
        return _componentColor;
    }

    td::ColorID getSelectedComponentColor() const
    {
        return _selectedComponentColor;
    }

    // IProperty
    virtual void getValues(gui::PropertyValues &propValues) const
    {
        td::Variant frequency(_frequency);
        setValueByKey(propValues, PropID::Frequency, frequency);

        td::Variant autoSolve(_autoSolve);
        setValueByKey(propValues, PropID::AutoSolve, autoSolve);

        td::Variant componentColor(_componentColor);
        setValueByKey(propValues, PropID::ComponentColor, componentColor);

        td::Variant selectedComponentColor(_selectedComponentColor);
        setValueByKey(propValues, PropID::SelectedComponentColor, selectedComponentColor);
    }

    virtual void setValues(gui::PropertyValues &propValues)
    {
        td::Variant frequency = getValueByKey(propValues, PropID::Frequency);
        frequency.getValue(_frequency);

        td::Variant autoSolve = getValueByKey(propValues, PropID::AutoSolve);
        autoSolve.getValue(_autoSolve);

        td::Variant componentColor = getValueByKey(propValues, PropID::ComponentColor);
        componentColor.getValue(_componentColor);

        td::Variant selectedComponentColor = getValueByKey(propValues, PropID::SelectedComponentColor);
        selectedComponentColor.getValue(_selectedComponentColor);
    }

    virtual void setValue(td::UINT4 key, gui::PropertyValues &propValues)
    {
        auto propValue = propValues.getLastChangedValue();
        PropID propID = (PropID)key;
        switch (propID)
        {
        case PropID::Frequency:
        {
            propValue.getValue(_frequency);
        }
        break;
        case PropID::AutoSolve:
        {
            propValue.getValue(_autoSolve);
        }
        break;
        case PropID::ComponentColor:
            propValue.getValue(_componentColor);
            break;
        case PropID::SelectedComponentColor:
            propValue.getValue(_selectedComponentColor);
            break;
        default:
        {
            assert(false);
        }
        }
    }

    void createPropertiesForEditor(gui::Properties &properties) const
    {
        properties.reserve(4);
        {
            using namespace gui;
            {
                td::String lbl(tr("Frequency"));
                td::Variant var(_frequency);
                td::String toolTip(tr("FrequencyTT"));
                auto &prop = properties.push_back();
                prop.set((td::UINT4)PropID::Frequency, lbl, var);
                prop.setPresentation(gui::Property::Presentation::Default);
                prop.setMinValue(0);
                prop.setMaxValue((double)IGridComponent::maxFrequency);
                prop.setDecimalPointsAndThSep(0, true);
            }

            {
                td::String lbl(tr("AutoSolve"));
                td::Variant var(_autoSolve);
                auto &prop = properties.push_back();
                prop.set((td::UINT4)PropID::AutoSolve, lbl, var);
            }

            {
                td::String lbl(tr("VisualParams"));
                auto &prop = properties.push_back();
                prop.setGroup(lbl);
            }

            {
                td::String lbl(tr("CompColor"));
                td::Variant var(_componentColor);
                auto &prop = properties.push_back();
                prop.set((td::UINT4)PropID::ComponentColor, lbl, var);
            }

            {
                td::String lbl(tr("SelCompColor"));
                td::Variant var(_selectedComponentColor);
                auto &prop = properties.push_back();
                prop.set((td::UINT4)PropID::SelectedComponentColor, lbl, var);
            }
        }

        assert(properties.size() == 5);
    }
};
