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
        : GridComponent(startNode, endNode, 40, 40, 4), _acVoltageSource(voltage)
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

        gui::Circle circle0(getStartPoint(), _width / 2);
        _componentShapes[0].createCircle(circle0);

        gui::Circle circle1({getStartPoint().x - _width / 8, getStartPoint().y}, _width / 8);
        _componentShapes[1].createArc(circle1, 180, 0, 1);

        gui::Circle circle2({getStartPoint().x + _width / 8, getStartPoint().y}, _width / 8);
        _componentShapes[2].createArc(circle2, 0, 180, 1);

        gui::Circle circle3({getStartPoint().x + _width / 2, getStartPoint().y - _width / 2}, 1);
        _componentShapes[3].createCircle(circle3);
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
        double x_F = x_E + _height / 2 * sin(theta);
        double y_F = y_E - _height / 2 * cos(theta);

        gui::Point point_C(x_C, y_C);
        gui::Point point_F(x_F, y_F);

        gui::Circle circle0(point_C, _width / 2);
        _componentShapes[0].updateCircleNodes(circle0);

        gui::Circle circle1({x_C - _width / 8, y_C}, _width / 8);
        _componentShapes[1].createArc(circle1, 180, 0, 1);

        gui::Circle circle2({x_C + _width / 8, y_C}, _width / 8);
        _componentShapes[2].createArc(circle2, 0, 180, 1);

        gui::Circle circle3(point_F, 1);
        _componentShapes[3].createCircle(circle3);
    }

    void initProperties(gui::Properties *properties) const override
    {
        {
            auto &prop = properties->push_back();
            prop.setGroup(gui::tr("parameters"));
        }

        td::Variant valV(_acVoltageSource.maxVoltage);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::MaxVoltage, gui::tr("maxVoltage"), valV);
        }

        td::Variant valFi(_acVoltageSource.phaseAngle);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::PhaseAngle, gui::tr("phaseAngle"), valFi);
            prop.setMinValue(-360);
            prop.setMaxValue(360);
        }

        GridComponent::initProperties(properties);
    }

    void getValues(gui::PropertyValues &propValues) const override
    {

        td::Variant valV(_acVoltageSource.maxVoltage);
        propValues.setValueByKey((td::UINT4)PropID::MaxVoltage, valV);

        td::Variant valFi(_acVoltageSource.phaseAngle);
        propValues.setValueByKey((td::UINT4)PropID::PhaseAngle, valFi);

        GridComponent::getValues(propValues);
    }

    void setValues(gui::PropertyValues &propValues) override
    {
        td::Variant maxVoltage = propValues.getValueByKey((td::UINT4)PropID::MaxVoltage);
        maxVoltage.getValue(_acVoltageSource.maxVoltage);

        td::Variant phaseAngle = propValues.getValueByKey((td::UINT4)PropID::PhaseAngle);
        phaseAngle.getValue(_acVoltageSource.phaseAngle);

        GridComponent::setValues(propValues);
    }
};
