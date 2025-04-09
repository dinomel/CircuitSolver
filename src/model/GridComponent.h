//
//  GridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 8. 4. 2025..
//

#pragma once
#include "IGridComponent.h"
#include "../core/Component.h"

class GridComponent : public IGridComponent
{

    enum class PropID : td::UINT4
    {
        X1 = 200,
        Y1,
        X2,
        Y2,
        FillColor,
        LineColor
    };

protected:
    gui::Point _startPoint;
    gui::Point _endPoint;
    gui::Shape _shape;
    double _width;
    double _height;
    td::ColorID _fillColor;
    td::ColorID _lineColor;

public:
    GridComponent(const gui::Point &initPoint, double width, double height, td::ColorID fillColor, td::ColorID lineColor)
        : _width(width), _height(height), _fillColor(fillColor), _lineColor(lineColor), _startPoint(initPoint), _endPoint(initPoint)
    {
    }

    virtual ~GridComponent()
    {
    }

    virtual Component *getComponent() = 0;

    virtual void draw() const
    {
        _shape.drawWire(_lineColor);
    }

    virtual void load(arch::ArchiveIn &ar)
    {
        ////        td::BYTE attr = 0 ; //(td::BYTE) _attribs;
        ////        td::BYTE lnPattern = 0; //(td::BYTE) _linePattern;
        //        arch::EnumLoader<gui::Shape::Attribs> attr(_attribs, gui::Shape::Attribs::LineAndFill, gui::Shape::Attribs::LineAndFill);
        //        arch::EnumLoader<td::LinePattern> lnPattern(_linePattern, td::LinePattern::NA, td::LinePattern::Solid);
        //        ar >> _lineWidth >> attr >> _fillColor >> _lineColor >> lnPattern;
        ////        _attribs = (gui::Shape::Attribs) attr;
        ////        _linePattern = (td::LinePattern) lnPattern;
    }

    virtual void save(arch::ArchiveOut &ar) const
    {
        ////        td::BYTE attr = (td::BYTE) _attribs;
        ////        td::BYTE lnPattern = (td::BYTE) _linePattern;
        //        arch::EnumSaver<gui::Shape::Attribs> attr(_attribs);
        //        arch::EnumSaver<td::LinePattern> lnPattern(_linePattern);
        //        ar << _lineWidth << attr << _fillColor << _lineColor << lnPattern;
    }

    virtual void release()
    {
        delete this;
    }

    void initProperties(gui::Properties *properties) const
    {

        // if (createGroup)
        {
            auto &prop = properties->push_back();
            prop.setGroup(gui::tr("VisualData"));
        }

        td::Variant valColor(td::colorID);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::FillColor, gui::tr("FillColor"), valColor);
        }

        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::LineColor, gui::tr("LineColor"), valColor);
        }

        // group
        {
            auto &prop = properties->push_back();
            prop.setGroup(gui::tr("SizeAndPosition"));
        }

        td::Variant val(_startPoint.x);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::X1, gui::tr("StartPointX"), val);
        }

        val = td::Variant(_startPoint.y);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Y1, gui::tr("StartPointY"), val);
        }

        val = td::Variant(_endPoint.x);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::X2, gui::tr("EndPointX"), val);
        }

        val = td::Variant(_endPoint.y);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Y2, gui::tr("EndPointY"), val);
        }
    }

    virtual void getValues(gui::PropertyValues &propValues) const
    {
        //        td::INT4 iAttr= (td::INT4) _attribs;
        //        td::Variant attribs(iAttr);
        //        propValues.setValueByKey((td::UINT4)PropID::Attribs, attribs);

        //        td::Variant fillColor(_fillColor);
        //        propValues.setValueByKey((td::UINT4)PropID::FillColor, fillColor);
        //
        //        td::Variant lineColor(_lineColor);
        //        propValues.setValueByKey((td::UINT4)PropID::LineColor, lineColor);

        //        td::Variant lineWidth(_lineWidth);
        //        propValues.setValueByKey((td::UINT4)PropID::LineWidth, lineWidth);

        //        td::Variant linePattern(_linePattern);
        //        propValues.setValueByKey((td::UINT4)PropID::LinePattern, linePattern);
    }
    virtual void setValues(gui::PropertyValues &propValues)
    {
        propValues.setUpdateCanvas(true);
        //        td::Variant attribs = propValues.getValueByKey((td::UINT4)PropID::Attribs);
        //        td::INT4 iAttr=0;
        //        attribs.getValue(iAttr);
        //        _attribs = (gui::Shape::Attribs) iAttr;

        //        td::Variant fillColor = propValues.getValueByKey((td::UINT4)PropID::FillColor);
        //        fillColor.getValue(_fillColor);
        //
        //        td::Variant lineColor = propValues.getValueByKey((td::UINT4)PropID::LineColor);
        //        lineColor.getValue(_lineColor);

        //        float lineWidth0 = _lineWidth;
        //        td::Variant lineWidth = propValues.getValueByKey((td::UINT4)PropID::LineWidth);
        //        lineWidth.getValue(_lineWidth);
        //        if (lineWidth0 != _lineWidth)
        //            _shape.setLineWidth(_lineWidth);

        //        td::LinePattern linePattern0 = _linePattern;
        //        td::Variant linePattern = propValues.getValueByKey((td::UINT4)PropID::LinePattern);
        //        linePattern.getValue(_linePattern);
        //
        //        if (linePattern0 != _linePattern)
        //            _shape.setLinePattern(_linePattern);
    }

    virtual void setValue(td::UINT4 key, gui::PropertyValues &propValues)
    {
    }
};
