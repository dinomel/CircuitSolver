//
//  InductorGridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 9. 4. 2025..
//

#pragma once
#include "GridComponent.h"
#include "../core/Inductor.h"
#include "../core/Component.h"
#include <numbers>

class InductorGridComponent : public GridComponent
{

protected:
    Inductor _inductor;

public:
    InductorGridComponent(double inductance, const gui::Point &initPoint, td::ColorID fillColor, td::ColorID lineColor)
        : GridComponent(initPoint, 48, 16, fillColor, lineColor), _inductor(inductance)
    {
    }

    virtual Component *getComponent()
    {
        return &_inductor;
    }

    virtual Type getType() const
    {
        return Type::Inductor;
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
            _startPoint,
            _endPoint,
            _startPoint,
            _endPoint,
            _startPoint,
            _endPoint,
            _startPoint,
            _endPoint,
        };
        _shape.createLines(&points[0], 28);
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
        double alpha = std::numbers::pi * 3 / 8 - theta;
        double sAlpha = sin(alpha);
        double cAlpha = cos(alpha);
        double beta = std::numbers::pi / 4 - theta;
        double sBeta = sin(beta);
        double cBeta = cos(beta);
        double gamma = std::numbers::pi * 3 / 8 + theta;
        double sGamma = sin(gamma);
        double cGamma = cos(gamma);
        double d_DF = _width / 3 * 0.3827;
        double d_DG = _width / 3 * 0.7071;

        double x_D = x_A + d_AD * cTheta;
        double y_D = y_A + d_AD * sTheta;
        double x_I = x_D + _width / 3 * cTheta;
        double y_I = y_D + _width / 3 * sTheta;
        double x_M = x_I + _width / 3 * cTheta;
        double y_M = y_I + _width / 3 * sTheta;
        double x_E = x_B - d_AD * cTheta;
        double y_E = y_B - d_AD * sTheta;

        double x_F = x_D + d_DF * cAlpha;
        double y_F = y_D - d_DF * sAlpha;
        double x_G = x_D + d_DG * cBeta;
        double y_G = y_D - d_DG * sBeta;
        double x_H = x_I - d_DF * cGamma;
        double y_H = y_I - d_DF * sGamma;

        double x_J = x_I + d_DF * cAlpha;
        double y_J = y_I - d_DF * sAlpha;
        double x_K = x_I + d_DG * cBeta;
        double y_K = y_I - d_DG * sBeta;
        double x_L = x_M - d_DF * cGamma;
        double y_L = y_M - d_DF * sGamma;

        double x_N = x_M + d_DF * cAlpha;
        double y_N = y_M - d_DF * sAlpha;
        double x_O = x_M + d_DG * cBeta;
        double y_O = y_M - d_DG * sBeta;
        double x_P = x_E - d_DF * cGamma;
        double y_P = y_E - d_DF * sGamma;

        gui::Point point_D(x_D, y_D);
        gui::Point point_F(x_F, y_F);
        gui::Point point_G(x_G, y_G);
        gui::Point point_H(x_H, y_H);

        gui::Point point_I(x_I, y_I);
        gui::Point point_J(x_J, y_J);
        gui::Point point_K(x_K, y_K);
        gui::Point point_L(x_L, y_L);

        gui::Point point_M(x_M, y_M);
        gui::Point point_N(x_N, y_N);
        gui::Point point_O(x_O, y_O);
        gui::Point point_P(x_P, y_P);

        gui::Point point_E(x_E, y_E);

        gui::Point points[] = {
            _startPoint,
            point_D,

            point_D,
            point_F,

            point_F,
            point_G,

            point_G,
            point_H,

            point_H,
            point_I,

            point_I,
            point_J,

            point_J,
            point_K,

            point_K,
            point_L,

            point_L,
            point_M,

            point_M,
            point_N,

            point_N,
            point_O,

            point_O,
            point_P,

            point_P,
            point_E,

            point_E,
            _endPoint,
        };
        _shape.createLines(&points[0], 28);
    }
};
