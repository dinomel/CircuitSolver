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
#define _USE_MATH_DEFINES
#include <cmath>

class InductorGridComponent : public GridComponent
{

protected:
    Inductor _inductor;

public:
    InductorGridComponent(double inductance, NodeGridComponent *startNode, NodeGridComponent *endNode)
        : GridComponent(startNode, endNode, 48, 16), _inductor(inductance)
    {
    }

    Component *getComponent() override
    {
        return &_inductor;
    }

    Type getType() const override
    {
        return Type::Inductor;
    }

    void init() override
    {
        gui::Point points[] = {
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
        };
        _shape.createLines(&points[0], 28);
    }

    void updateShape() override
    {
        double x_A = getStartPoint().x;
        double y_A = getStartPoint().y;
        double x_B = getEndPoint().x;
        double y_B = getEndPoint().y;
        double d_AB = std::sqrt((x_B - x_A) * (x_B - x_A) + (y_B - y_A) * (y_B - y_A));
        double d_AD = (d_AB - _width) / 2;
        double theta = std::atan2(y_B - y_A, x_B - x_A);
        double sTheta = sin(theta);
        double cTheta = cos(theta);
        double alpha = M_PI * 3 / 8 - theta;
        double sAlpha = sin(alpha);
        double cAlpha = cos(alpha);
        double beta = M_PI / 4 - theta;
        double sBeta = sin(beta);
        double cBeta = cos(beta);
        double gamma = M_PI * 3 / 8 + theta;
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
            getStartPoint(),
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
            getEndPoint(),
        };
        _shape.createLines(&points[0], 28);
    }

    void initProperties(gui::Properties *properties) const override
    {
        // if (createGroup)
        {
            auto &prop = properties->push_back();
            prop.setGroup("Parameters");
        }

        td::Variant valL(_inductor.inductance);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Inductance, "Inductance", valL);
        }

        initNodesProperties(properties);
    }

    void getValues(gui::PropertyValues &propValues) const override
    {

        td::Variant valL(_inductor.inductance);
        propValues.setValueByKey((td::UINT4)PropID::Inductance, valL);

        getNodesValues(propValues);
    }

    void setValues(gui::PropertyValues &propValues) override
    {
        td::Variant inductance = propValues.getValueByKey((td::UINT4)PropID::Inductance);
        inductance.getValue(_inductor.inductance);

        setNodesValues(propValues);

        propValues.setUpdateCanvas(true);
        startNode->updateShape();
        endNode->updateShape();
        updateShape();
    }
};
