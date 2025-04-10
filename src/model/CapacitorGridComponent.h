//
//  CapacitorGridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 8. 4. 2025..
//

#pragma once
#include "GridComponent.h"
#include "../core/Component.h"
#include "../core/Capacitor.h"

class CapacitorGridComponent : public GridComponent
{
protected:
    Capacitor _capacitor;

public:
    CapacitorGridComponent(double capacitance, const gui::Point &initPoint, td::ColorID fillColor, td::ColorID lineColor)
        : GridComponent(initPoint, 20, 40, fillColor, lineColor), _capacitor(capacitance)
    {
    }

    virtual Component *getComponent()
    {
        return &_capacitor;
    }

    virtual Type getType() const
    {
        return Type::Capacitor;
    }

    void init()
    {
        gui::Point points[] = {
            _startPoint,
            _endPoint,
            _startPoint,
            _endPoint,
            _startPoint,
            _endPoint,
            _startPoint,
            _endPoint,
        };
        _shape.createLines(&points[0], 8);
    }

    virtual void getBoundingRect(gui::Rect &boundRect)
    {
        boundRect = gui::Rect(_startPoint.x, _startPoint.y, _endPoint.x, _endPoint.y);
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
        //        td::Variant x(_rect.left);
        //        propValues.setValueByKey((td::UINT4)PropID::Xr, x);
        //
        //        td::Variant y(_rect.top);
        //        propValues.setValueByKey((td::UINT4)PropID::Yr, y);
        //
        //        td::Variant w(_rect.width());
        //        propValues.setValueByKey((td::UINT4)PropID::Width, w);
        //
        //        td::Variant h(_rect.height());
        //        propValues.setValueByKey((td::UINT4)PropID::Height, h);
        //
        //        GridComponent::getValues(propValues);
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
            point_I,
            point_H,
            point_E,
            _endPoint,
        };
        _shape.createLines(&points[0], 8);
    }
};
