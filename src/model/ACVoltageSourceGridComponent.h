//
//  ACVoltageSourceGridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 15. 4. 2025..
//

#pragma once
#include "GridComponent.h"
#include "../core/ACVoltageSource.h"
#include "../core/Component.h"

class ACVoltageSourceGridComponent : public GridComponent
{
protected:
    ACVoltageSource _acVoltageSource;

public:
    ACVoltageSourceGridComponent(double voltage, NodeGridComponent *startNode, NodeGridComponent *endNode)
        : GridComponent(startNode, endNode, 40, 40, 2), _acVoltageSource(voltage)
    {
    }

    Component *getComponent() override
    {
        return &_acVoltageSource;
    }

    Type getType() const override
    {
        return Type::ACVoltageSource;
    }

    void init() override
    {
        GridComponent::init();

        gui::Circle circle(getStartPoint(), _width / 2);
        _componentShapes[0].createCircle(circle);

        double x = getStartPoint().x;
        double y = getStartPoint().y;
        gui::Point points[] = {
            {x - _width / 4, y},
            {x - _width / 8, y - _width / 4},
            {x + _width / 8, y + _width / 4},
            {x + _width / 4, y},
        };
        _componentShapes[1].createPolyLine(&points[0], 4);
    }

    void updateShape() override
    {
        auto [point_D, point_E, theta] = updateWiresShape();
        double x_D = point_D.x;
        double y_D = point_D.y;
        double x_E = point_E.x;
        double y_E = point_E.y;

        double x_C = (x_D + x_E) / 2;
        double y_C = (y_D + y_E) / 2;

        gui::Point point_C(x_C, y_C);

        gui::Circle circle(point_C, _width / 2);
        _componentShapes[0].updateCircleNodes(circle);

        gui::Point points[] = {
            {x_C - _width / 4, y_C},
            {x_C - _width / 8, y_C - _width / 4},
            {x_C + _width / 8, y_C + _width / 4},
            {x_C + _width / 4, y_C},
        };
        _componentShapes[1].createPolyLine(&points[0], 4);
    }

    void initProperties(gui::Properties *properties) const override
    {
        // if (createGroup)
        // {
        //     auto &prop = properties->push_back();
        //     prop.setGroup("Parameters");
        // }

        // td::Variant valR(_resistor.resistance);
        // {
        //     auto &prop = properties->push_back();
        //     prop.set((td::UINT4)PropID::Resistance, "Resistance", valR);
        // }

        // valR = td::Variant(_resistor.reactance);
        // {
        //     auto &prop = properties->push_back();
        //     prop.set((td::UINT4)PropID::Reactance, "Reactance", valR);
        // }

        GridComponent::initProperties(properties);
    }

    void getValues(gui::PropertyValues &propValues) const override
    {

        // td::Variant valR(_resistor.resistance);
        // propValues.setValueByKey((td::UINT4)PropID::Resistance, valR);

        // td::Variant valX(_resistor.reactance);
        // propValues.setValueByKey((td::UINT4)PropID::Reactance, valX);

        GridComponent::getValues(propValues);
    }

    void setValues(gui::PropertyValues &propValues) override
    {
        // td::Variant resistance = propValues.getValueByKey((td::UINT4)PropID::Resistance);
        // resistance.getValue(_resistor.resistance);

        // td::Variant reactance = propValues.getValueByKey((td::UINT4)PropID::Reactance);
        // reactance.getValue(_resistor.reactance);

        GridComponent::setValues(propValues);
    }
};
