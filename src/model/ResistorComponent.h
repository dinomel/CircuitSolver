//
//  ResistorComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 8. 4. 2025..
//

#pragma once
#include "GridComponent.h"

class ResistorComponent : public GridComponent
{
    enum class PropID : td::UINT4
    {
        Xr = 200,
        Yr,
        Width,
        Height
    };

public:
    ResistorComponent(const gui::Point &initPoint, td::ColorID fillColor, td::ColorID lineColor)
        : GridComponent(initPoint, 40, 16, fillColor, lineColor)
    {
        //        _endPoint.translate(150, 120);
    }

    virtual Type getType() const
    {
        return Type::Resistor;
    }

    void init()
    {
        //        gui::Point points[] = {
        //            {0, 12}, {16, 12},
        //            {16, 0}, {16, 24},
        //            {24, 0}, {24, 24},
        //            {24, 12}, {40, 12},
        //            {40, 5}, {40, 19},
        //            {43, 7}, {43, 17},
        ////            {46, 10}, {46, 14},
        //        };
        //        _shape.createLines(&points[0], 14);

        //        if (r.width() > 0 && r.height() > 0)
        //        _shape.createRect(r, _lineWidth, _linePattern);
        //        _shape.createPolyLine(&points[0], 11);

        gui::Point points[] = {
            _startPoint,
            _endPoint,
            _startPoint,
            _endPoint,
            _startPoint,
            _endPoint,
            _startPoint,
            _endPoint,
            _startPoint,
            _endPoint,
            _startPoint,
            _endPoint,
        };
        //
        _shape.createLines(&points[0], 12);
        //        _shape.createPolyLine(&points[0], 12);
    }

    virtual void getBoundingRect(gui::Rect &boundRect)
    {
        boundRect = gui::Rect(_startPoint.x, _startPoint.y, _endPoint.x, _endPoint.y);
    }

    virtual bool containsPoint(const gui::Point &pt) const
    {
        return gui::Rect(_startPoint.x, _startPoint.y, _endPoint.x, _endPoint.y).contains(pt);
    }

    virtual void load(arch::ArchiveIn &ar)
    {
        GridComponent::load(ar);
        ar >> _startPoint;
    }

    virtual void save(arch::ArchiveOut &ar) const
    {
        GridComponent::save(ar);
        ar << _startPoint;
    }

    virtual void getValues(gui::PropertyValues &propValues) const
    {
        //                td::Variant x1(_startPoint.x);
        //                propValues.setValueByKey((td::UINT4)PropID::Xr, x1);
        //
        //                td::Variant y1(_startPoint.y);
        //                propValues.setValueByKey((td::UINT4)PropID::Yr, y1);
        //
        //                td::Variant x2(_endPoint.x);
        //                propValues.setValueByKey((td::UINT4)PropID::Width, x2);
        //
        //                td::Variant y2(_endPoint.y);
        //                propValues.setValueByKey((td::UINT4)PropID::Height, y2);
        //
        //                GridComponent::getValues(propValues);
    }

    virtual void setValues(gui::PropertyValues &propValues)
    {
        //        gui::Rect rBefore(_rect);
        //        if (isWithLine())
        //            rBefore.inflate(_lineWidth+IGridComponent::refreshOffset);
        //
        //        propValues.setRectBeforeUpdate(rBefore);
        //
        //        td::Variant x = propValues.getValueByKey((td::UINT4)PropID::Xr);
        //        x.getValue(_rect.left);
        //
        //        td::Variant y = propValues.getValueByKey((td::UINT4)PropID::Yr);
        //        y.getValue(_rect.top);
        //
        //        td::Variant varW = propValues.getValueByKey((td::UINT4)PropID::Width);
        //        gui::CoordType w;
        //        varW.getValue(w);
        //        _rect.setWidth(w);
        //
        //        td::Variant varH = propValues.getValueByKey((td::UINT4)PropID::Height);
        //        gui::CoordType h;
        //        varH.getValue(h);
        //        _rect.setHeight(h);
        //        gui::Rect rectAfter(_rect);
        //
        //        GridComponent::setValues(propValues);
        //        gui::Rect rAfter(_rect);
        //
        //        if (isWithLine())
        //            rAfter.inflate(_lineWidth+IGridComponent::refreshOffset);
        //        propValues.setRectAfterUpdate(rAfter);
        //
        //        if (getType() == IGridComponent::Type::Rect)
        //        {
        //            if (rBefore != rAfter)
        //                _shape.updateRectNodes(_rect);
        //        }
    }

    virtual bool canBeSelected(const gui::Point &pt) const
    {
        //        if (_rect.contains(pt))
        //        {
        //            if (isWithFill())
        //                return true;
        //            auto minDistance = _rect.getMinDistance2(pt);
        //            if (minDistance <= IGridComponent::selectionDisance2)
        //                return true;
        //        }
        return false;
    }

    virtual void translate(const gui::Point &delta)
    {

        //        _rect.translate(delta);
        //        _shape.translateRectNodes(delta);
        // or
        //_shape.updateRectNodes(_rect);
    }

    virtual void updateEndPoint(const gui::Point &newEndPoint)
    {
        _endPoint = newEndPoint;
        updateLineNodes();
        //        gui::Rect r(_startPoint.x, _startPoint.y, _endPoint.x, _endPoint.y);
        //        _shape.updateRectNodes(r);
        //        gui::Point points[] = {_startPoint, {_startPoint.x+10, _startPoint.y-10}, _endPoint, {_endPoint.x-10, _endPoint.y+10}};
    }

    virtual void updateLineNodes()
    {
        double x_A = _startPoint.x;
        double y_A = _startPoint.y;
        double x_B = _endPoint.x;
        double y_B = _endPoint.y;

        double d_AB = std::sqrt((x_B - x_A) * (x_B - x_A) + (y_B - y_A) * (y_B - y_A));
        double d_AD = (d_AB - _width) / 2;
        double theta = std::atan2(y_B - y_A, x_B - x_A);
        double sTheta = sin(theta);
        double cTheta = cos(theta);

        double x_D = x_A + d_AD * cTheta;
        double y_D = y_A + d_AD * sTheta;
        double x_E = x_B - d_AD * cTheta;
        double y_E = y_B - d_AD * sTheta;
        double halfHeight = _height / 2;
        double x_G = x_D + halfHeight * sTheta;
        double y_G = y_D - halfHeight * cTheta;
        double x_F = x_D - halfHeight * sTheta;
        double y_F = y_D + halfHeight * cTheta;
        double x_I = x_E + halfHeight * sTheta;
        double y_I = y_E - halfHeight * cTheta;
        double x_H = x_E - halfHeight * sTheta;
        double y_H = y_E + halfHeight * cTheta;

        gui::Point point_D(x_D, y_D);
        gui::Point point_E(x_E, y_E);
        gui::Point point_G(x_G, y_G);
        gui::Point point_F(x_F, y_F);
        gui::Point point_H(x_H, y_H);
        gui::Point point_I(x_I, y_I);

        gui::Point points[] = {
            _startPoint,
            point_D,
            point_F,
            point_G,
            point_G,
            point_I,
            point_I,
            point_H,
            point_H,
            point_F,
            point_E,
            _endPoint,
        };
        _shape.createLines(&points[0], 12);
    }
};
