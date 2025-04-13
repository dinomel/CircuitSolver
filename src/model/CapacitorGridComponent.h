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
    CapacitorGridComponent(double capacitance, NodeGridComponent *startNode, NodeGridComponent *endNode)
        : GridComponent(startNode, endNode, 20, 40), _capacitor(capacitance)
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
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
        };
        _shape.createLines(&points[0], 8);
    }

    virtual void updateShape()
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
            getStartPoint(),
            point_D,
            point_F,
            point_G,
            point_I,
            point_H,
            point_E,
            getEndPoint(),
        };
        _shape.createLines(&points[0], 8);
    }

    virtual void initProperties(gui::Properties *properties) const
    {
        // if (createGroup)
        {
            auto &prop = properties->push_back();
            prop.setGroup("Parameters");
        }

        td::Variant valC(_capacitor.capacitance);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Capacitance, "Capacitance", valC);
        }

        initNodesProperties(properties);
    }

    virtual void getValues(gui::PropertyValues &propValues) const
    {

        td::Variant valC(_capacitor.capacitance);
        propValues.setValueByKey((td::UINT4)PropID::Capacitance, valC);

        getNodesValues(propValues);
    }

    virtual void setValues(gui::PropertyValues &propValues)
    {
        td::Variant capacitance = propValues.getValueByKey((td::UINT4)PropID::Capacitance);
        capacitance.getValue(_capacitor.capacitance);

        setNodesValues(propValues);

        propValues.setUpdateCanvas(true);
        startNode->updateShape();
        endNode->updateShape();
        updateShape();
    }
};
