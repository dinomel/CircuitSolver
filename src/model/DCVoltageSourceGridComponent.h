//
//  DCVoltageSourceGridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 15. 4. 2025..
//

#pragma once
#include "GridComponent.h"
#include "../core/DCVoltageSource.h"
#include "../core/Component.h"

class DCVoltageSourceGridComponent : public GridComponent
{
protected:
    DCVoltageSource _dcVoltageSource;

public:
    DCVoltageSourceGridComponent(double voltage, NodeGridComponent *startNode, NodeGridComponent *endNode)
        : GridComponent(startNode, endNode, 12, 36), _dcVoltageSource(voltage)
    {
    }

    Component *getComponent() override
    {
        return &_dcVoltageSource;
    }

    Type getType() const override
    {
        return Type::DCVoltageSource;
    }

    void init() override
    {
        GridComponent::init();
        gui::Point points[] = {
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
        };
        _componentShapes[0].createLines(&points[0], 4);
    }

    void updateShape() override
    {
        auto [point_D, point_E, theta] = updateWiresShape();
        double x_D = point_D.x;
        double y_D = point_D.y;
        double x_E = point_E.x;
        double y_E = point_E.y;

        double sTheta = sin(theta);
        double cTheta = cos(theta);
        double halfHeight = _height / 2;
        double quarterHeight = _height / 4;
        double x_G = x_D + quarterHeight * sTheta;
        double y_G = y_D - quarterHeight * cTheta;
        double x_F = x_D - quarterHeight * sTheta;
        double y_F = y_D + quarterHeight * cTheta;
        double x_I = x_E + halfHeight * sTheta;
        double y_I = y_E - halfHeight * cTheta;
        double x_H = x_E - halfHeight * sTheta;
        double y_H = y_E + halfHeight * cTheta;

        gui::Point point_G(x_G, y_G);
        gui::Point point_F(x_F, y_F);
        gui::Point point_H(x_H, y_H);
        gui::Point point_I(x_I, y_I);

        gui::Point points[] = {
            point_F,
            point_G,
            point_I,
            point_H,
        };
        _componentShapes[0].createLines(&points[0], 4);
    }

    void initProperties(gui::Properties *properties) const override
    {
        {
            auto &prop = properties->push_back();
            prop.setGroup(gui::tr("parameters"));
        }

        td::Variant valV(_dcVoltageSource.voltage);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Voltage, gui::tr("voltage"), valV);
        }

        GridComponent::initProperties(properties);
    }

    void getValues(gui::PropertyValues &propValues) const override
    {

        td::Variant valV(_dcVoltageSource.voltage);
        propValues.setValueByKey((td::UINT4)PropID::Voltage, valV);

        GridComponent::getValues(propValues);
    }

    void setValues(gui::PropertyValues &propValues) override
    {
        td::Variant voltage = propValues.getValueByKey((td::UINT4)PropID::Voltage);
        voltage.getValue(_dcVoltageSource.voltage);

        GridComponent::setValues(propValues);
    }
};
