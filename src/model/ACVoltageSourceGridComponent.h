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
    gui::Shape _shapeTest1;
    gui::Shape _shapeTest2;
    ACVoltageSource _acVoltageSource;

public:
    ACVoltageSourceGridComponent(double voltage, NodeGridComponent *startNode, NodeGridComponent *endNode)
        : GridComponent(startNode, endNode, 40, 40), _acVoltageSource(voltage)
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

    void draw() const override
    {
        _shapeTest1.drawWire(td::ColorID::Yellow);
        _shapeTest2.drawWire(td::ColorID::Yellow);
        GridComponent::draw();
    }

    void init() override
    {
        gui::Point points[] = {
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
        };
        _shapeTest1.createLines(&points[0], 4);

        gui::Circle circle(getStartPoint(), _width / 2);
        _shape.createCircle(circle);

        double x = getStartPoint().x;
        double y = getStartPoint().y;
        gui::Point points2[] = {
            {x - _width / 4, y},
            {x - _width / 8, y - _width / 4},
            {x + _width / 8, y + _width / 4},
            {x + _width / 4, y},
        };
        _shapeTest2.createPolyLine(&points2[0], 4);
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

        double x_C = (x_A + x_B) / 2;
        double y_C = (y_A + y_B) / 2;
        double x_D = x_A + d_AD * cTheta;
        double y_D = y_A + d_AD * sTheta;
        double x_E = x_B - d_AD * cTheta;
        double y_E = y_B - d_AD * sTheta;

        gui::Point point_C(x_C, y_C);
        gui::Point point_D(x_D, y_D);
        gui::Point point_E(x_E, y_E);

        gui::Point points[] = {
            getStartPoint(),
            point_D,
            point_E,
            getEndPoint(),
        };
        _shapeTest1.createLines(&points[0], 4);

        gui::Circle circle(point_C, _width / 2);
        _shape.updateCircleNodes(circle);

        gui::Point points2[] = {
            {x_C - _width / 4, y_C},
            {x_C - _width / 8, y_C - _width / 4},
            {x_C + _width / 8, y_C + _width / 4},
            {x_C + _width / 4, y_C},
        };
        _shapeTest2.createPolyLine(&points2[0], 4);
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
