//
//  CurrentSourceGridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 17. 4. 2025..
//

#pragma once
#include "GridComponent.h"
#include "../core/CurrentSource.h"
#include "../core/Component.h"
#define _USE_MATH_DEFINES

class CurrentSourceGridComponent : public GridComponent
{
protected:
    CurrentSource _currentSource;

public:
    CurrentSourceGridComponent(double current, NodeGridComponent *startNode, NodeGridComponent *endNode)
        : GridComponent(startNode, endNode, 40, 40, 2), _currentSource(current)
    {
    }

    Component *getComponent() override
    {
        return &_currentSource;
    }

    Type getType() const override
    {
        return Type::CurrentSource;
    }

    void init() override
    {
        GridComponent::init();

        gui::Circle circle(getStartPoint(), _width / 2);
        _componentShapes[0].createCircle(circle);

        gui::Point points[] = {
            {getStartPoint().x - _width / 4, getStartPoint().y},
            {getStartPoint().x + _width / 4, getStartPoint().y},
            {getStartPoint().x + _width / 4, getStartPoint().y},
            {getStartPoint().x + _width / 8, getStartPoint().y - _width / 8},
            {getStartPoint().x + _width / 4, getStartPoint().y},
            {getStartPoint().x + _width / 8, getStartPoint().y + _width / 8},
        };
        _componentShapes[1].createLines(&points[0], 6);
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
        double x_F = x_C - _width / 4 * cos(theta);
        double y_F = y_C - _width / 4 * sin(theta);
        double x_G = x_C + _width / 4 * cos(theta);
        double y_G = y_C + _width / 4 * sin(theta);

        double l = _width * 0.17677669529; // sqrt(1/32)
        double theta2 = M_PI / 4 - theta;
        double x_H = x_G - l * sin(theta2);
        double y_H = y_G - l * cos(theta2);
        double x_I = x_G - l * cos(theta2);
        double y_I = y_G + l * sin(theta2);

        gui::Point point_C(x_C, y_C);
        gui::Point point_F(x_F, y_F);
        gui::Point point_G(x_G, y_G);
        gui::Point point_H(x_H, y_H);
        gui::Point point_I(x_I, y_I);

        gui::Circle circle(point_C, _width / 2);
        _componentShapes[0].updateCircleNodes(circle);

        gui::Point points[] = {
            point_F,
            point_G,
            point_H,
            point_G,
            point_I,
            point_G,
        };
        _componentShapes[1].createLines(&points[0], 6);
    }

    void initProperties(gui::Properties *properties) const override
    {
        {
            auto &prop = properties->push_back();
            prop.setGroup("Parameters");
        }

        td::Variant valI(_currentSource.current);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Current, "Current", valI);
        }

        GridComponent::initProperties(properties);
    }

    void getValues(gui::PropertyValues &propValues) const override
    {

        td::Variant valI(_currentSource.current);
        propValues.setValueByKey((td::UINT4)PropID::Current, valI);

        GridComponent::getValues(propValues);
    }

    void setValues(gui::PropertyValues &propValues) override
    {
        td::Variant current = propValues.getValueByKey((td::UINT4)PropID::Current);
        current.getValue(_currentSource.current);

        GridComponent::setValues(propValues);
    }
};
