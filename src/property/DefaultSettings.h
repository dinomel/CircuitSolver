//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Shape.h>
#include <gui/IProperty.h>
#include <gui/Properties.h>
#include "../model/IGridComponent.h"

class DefaultSettings : public gui::IProperty
{
    enum class PropID : td::UINT4
    {
        LineWidth = 100,
        RectWidth,
        RectHeight,
        CircRadius,
        RoundedRectRadius,
        Attribs = 200,
        FillColor,
        LineColor,
        LinePattern,
        TestBool
    };

protected:
    float _lineWidth = 2;
    float _rectWidth = 50;
    float _rectHeight = 20;
    float _circRadius = 25;
    float _roundedRectRadius = 5;
    bool _testBool = true;
    gui::Shape::Attribs _attribs = gui::Shape::Attribs::LineAndFill;
    td::ColorID _lineColor = td::ColorID::Yellow;
    td::ColorID _fillColor = td::ColorID::Blue;
    td::LinePattern _linePattern = td::LinePattern::Dash;

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
    float getLineWidth() const
    {
        return _lineWidth;
    }
    float getRectWidth() const
    {
        return _rectWidth;
    }
    float getRectHeight() const
    {
        return _rectHeight;
    }
    float getCircleRadius() const
    {
        return _circRadius;
    }

    float getRoundedRectRadius() const
    {
        return _roundedRectRadius;
    }

    gui::Shape::Attribs getAttribs() const
    {
        return _attribs;
    }

    td::ColorID getLineColor() const
    {
        return _lineColor;
    }

    td::ColorID getFillColor() const
    {
        return _fillColor;
    }

    td::LinePattern getLinePattern() const
    {
        return _linePattern;
    }

    // IProperty
    virtual void getValues(gui::PropertyValues &propValues) const
    {
        td::Variant lineWidth(_lineWidth);
        setValueByKey(propValues, PropID::LineWidth, lineWidth);

        td::Variant rectWidth(_rectWidth);
        setValueByKey(propValues, PropID::RectWidth, rectWidth);

        td::Variant rectHeight(_rectHeight);
        setValueByKey(propValues, PropID::RectHeight, rectHeight);

        td::Variant circRadius(_circRadius);
        setValueByKey(propValues, PropID::CircRadius, circRadius);

        td::Variant roundRectRadius(_roundedRectRadius);
        setValueByKey(propValues, PropID::RoundedRectRadius, roundRectRadius);

        td::INT4 iAttr = (td::INT4)_attribs;
        td::Variant attribs(iAttr);
        setValueByKey(propValues, PropID::Attribs, attribs);

        td::Variant lineColor(_lineColor);
        setValueByKey(propValues, PropID::LineColor, lineColor);

        td::Variant fillColor(_fillColor);
        setValueByKey(propValues, PropID::FillColor, _fillColor);

        td::Variant linePattern(_linePattern);
        setValueByKey(propValues, PropID::LinePattern, linePattern);

        td::Variant testBool(_testBool);
        setValueByKey(propValues, PropID::TestBool, testBool);
    }

    virtual void setValues(gui::PropertyValues &propValues)
    {
        td::Variant lineWidth = getValueByKey(propValues, PropID::LineWidth);
        lineWidth.getValue(_lineWidth);

        td::Variant rectWidth = getValueByKey(propValues, PropID::RectWidth);
        rectWidth.getValue(_rectWidth);

        td::Variant rectHeight = getValueByKey(propValues, PropID::RectHeight);
        rectHeight.getValue(_rectHeight);

        td::Variant circRadius = getValueByKey(propValues, PropID::CircRadius);
        circRadius.getValue(_circRadius);

        td::Variant roundRectRadius = getValueByKey(propValues, PropID::RoundedRectRadius);
        roundRectRadius.getValue(_roundedRectRadius);

        td::Variant attribs = getValueByKey(propValues, PropID::Attribs);
        td::INT4 iAttr = 0;
        attribs.getValue(iAttr);
        _attribs = (gui::Shape::Attribs)iAttr;

        td::Variant lineColor = getValueByKey(propValues, PropID::LineColor);
        lineColor.getValue(_lineColor);

        td::Variant fillColor = getValueByKey(propValues, PropID::FillColor);
        fillColor.getValue(_fillColor);

        td::Variant linePattern = getValueByKey(propValues, PropID::LinePattern);
        linePattern.getValue(_linePattern);

        td::Variant testBool = getValueByKey(propValues, PropID::TestBool);
        testBool.getValue(_testBool);
    }

    virtual void setValue(td::UINT4 key, gui::PropertyValues &propValues)
    {
        auto propValue = propValues.getLastChangedValue();
        PropID propID = (PropID)key;
        switch (propID)
        {
        case PropID::LineWidth:
        {
            // slider returns double value
            double dblVal;
            propValue.getValue(dblVal);
            _lineWidth = (float)dblVal;
        }
        break;
        case PropID::RectWidth:
            propValue.getValue(_rectWidth);
            break;
        case PropID::RectHeight:
            propValue.getValue(_rectHeight);
            break;
        case PropID::CircRadius:
            propValue.getValue(_circRadius);
            break;
        case PropID::RoundedRectRadius:
            propValue.getValue(_roundedRectRadius);
            break;
        case PropID::Attribs:
        {
            // comvert int4 from combo to attribs
            td::INT4 attribs;
            propValue.getValue(attribs);
            _attribs = (gui::Shape::Attribs)attribs;
        }
        break;
        case PropID::LineColor:
            propValue.getValue(_lineColor);
            break;
        case PropID::FillColor:
            propValue.getValue(_fillColor);
            break;
        case PropID::LinePattern:
            propValue.getValue(_linePattern);
            break;
        case PropID::TestBool:
            propValue.getValue(_testBool);
            break;

        default:
        {
            assert(false);
        }
        }
    }

    void createPropertiesForEditor(gui::Properties &properties) const
    {
        properties.reserve(12);
        {
            using namespace gui;
            // first group
            {
                td::String lbl(tr("GeomData"));
                auto &prop = properties.push_back();
                prop.setGroup(lbl);
            }

            {
                td::String lbl(tr("RectWidth"));
                td::Variant var(_rectWidth);
                td::String toolTip(tr("RectWidthTT"));
                auto &prop = properties.push_back();
                prop.set((td::UINT4)PropID::RectWidth, lbl, var, toolTip);
                prop.setDecimalPointsAndThSep(1, true);
            }

            {
                td::String lbl(tr("RectHeight"));
                td::Variant var(_rectHeight);
                td::String toolTip(tr("RectHeightTT"));
                auto &prop = properties.push_back();
                prop.set((td::UINT4)PropID::RectHeight, lbl, var, toolTip);
                prop.setDecimalPointsAndThSep(1, true);
            }

            {
                td::String lbl(tr("RoundRadius"));
                td::Variant var(_roundedRectRadius);
                td::String toolTip(tr("RoundRadiusTT"));
                auto &prop = properties.push_back();
                prop.set((td::UINT4)PropID::RoundedRectRadius, lbl, var, toolTip);
                prop.setDecimalPointsAndThSep(1, true);
            }

            {
                td::String lbl(tr("LineWidth"));
                td::Variant var(_lineWidth);
                td::String toolTip(tr("LineWidthTT"));
                auto &prop = properties.push_back();
                prop.set((td::UINT4)PropID::LineWidth, lbl, var, toolTip);
                prop.setPresentation(gui::Property::Presentation::Slider);
                prop.setMinValue(0);
                prop.setMaxValue((double)IGridComponent::maxLineWidth);
                // prop.setDecimalPointsAndThSep(1, true);
            }

            {
                td::String lbl(tr("CircleRadius"));
                td::Variant var(_circRadius);
                td::String toolTip(tr("CircleRadiusTT"));
                auto &prop = properties.push_back();
                prop.set((td::UINT4)PropID::CircRadius, lbl, var, toolTip);
                prop.setDecimalPointsAndThSep(1, true);
            }

            // second group
            {
                td::String lbl(tr("VisualData"));
                auto &prop = properties.push_back();
                prop.setGroup(lbl);
            }

            {
                td::String lbl(tr("Attribs"));
                td::Variant var(td::int4);
                auto &prop = properties.push_back();
                prop.set((td::UINT4)PropID::Attribs, lbl, var);
                prop.setPresentation(gui::Property::Presentation::Combo);
                td::String *pStrItems = IGridComponent::getAttribsDesc();
                prop.setStringList(pStrItems, 3);
            }

            {
                td::String lbl(tr("FillColor"));
                td::Variant var(_fillColor);
                auto &prop = properties.push_back();
                prop.set((td::UINT4)PropID::FillColor, lbl, var);
            }

            {
                td::String lbl(tr("LineColor"));
                td::Variant var(_lineColor);
                auto &prop = properties.push_back();
                prop.set((td::UINT4)PropID::LineColor, lbl, var);
            }

            {
                td::String lbl(tr("LinePattern"));
                td::Variant var(_linePattern);
                auto &prop = properties.push_back();
                prop.set((td::UINT4)PropID::LinePattern, lbl, var);
            }

            {
                td::String lbl(tr("TestBool"));
                td::Variant var(_testBool);
                auto &prop = properties.push_back();
                prop.set((td::UINT4)PropID::TestBool, lbl, var);
            }
        }

        assert(properties.size() == 12);
    }
};
