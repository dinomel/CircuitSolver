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
        : GridComponent(startNode, endNode, 48, 16, 3), _inductor(inductance)
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
        GridComponent::init();

        gui::Circle circle0({getStartPoint().x - _width / 3, getStartPoint().y}, _width / 6);
        _componentShapes[0].createArc(circle0, 180, 0, 1);

        gui::Circle circle1(getStartPoint(), _width / 6);
        _componentShapes[1].createArc(circle1, 180, 0, 1);

        gui::Circle circle2({getStartPoint().x + _width / 3, getStartPoint().y}, _width / 6);
        _componentShapes[2].createArc(circle2, 180, 0, 1);
    }

    void updateShape() override
    {
        auto [point_D, point_E, theta] = updateWiresShape();
        double x_D = point_D.x;
        double y_D = point_D.y;
        double x_E = point_E.x;
        double y_E = point_E.y;

        double thetaDeg = theta * 180 / M_PI;

        double x_C = (x_D + x_E) / 2;
        double y_C = (y_D + y_E) / 2;
        double x_F = (5 * x_D + x_E) / 6;
        double y_F = (5 * y_D + y_E) / 6;
        double x_G = (x_D + 5 * x_E) / 6;
        double y_G = (y_D + 5 * y_E) / 6;

        gui::Point point_C(x_C, y_C);
        gui::Point point_F(x_F, y_F);
        gui::Point point_G(x_G, y_G);

        gui::Circle circle0(point_F, _width / 6);
        _componentShapes[0].createArc(circle0, 180 + thetaDeg, thetaDeg, 1);

        gui::Circle circle1(point_C, _width / 6);
        _componentShapes[1].createArc(circle1, 180 + thetaDeg, thetaDeg, 1);

        gui::Circle circle2(point_G, _width / 6);
        _componentShapes[2].createArc(circle2, 180 + thetaDeg, thetaDeg, 1);
    }

    void initProperties(gui::Properties *properties) const override
    {
        {
            auto &prop = properties->push_back();
            prop.setGroup(gui::tr("parameters"));
        }

        td::Variant valL(_inductor.inductance);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Inductance, gui::tr("inductance"), valL);
        }

        GridComponent::initProperties(properties);
    }

    void getValues(gui::PropertyValues &propValues) const override
    {

        td::Variant valL(_inductor.inductance);
        propValues.setValueByKey((td::UINT4)PropID::Inductance, valL);

        GridComponent::getValues(propValues);
    }

    void setValues(gui::PropertyValues &propValues) override
    {
        td::Variant inductance = propValues.getValueByKey((td::UINT4)PropID::Inductance);
        inductance.getValue(_inductor.inductance);

        GridComponent::setValues(propValues);
    }

    void save(arch::ArchiveOut &ar) const override
    {
        GridComponent::save(ar);
        ar << _inductor.inductance;
    }

    void load(arch::ArchiveIn &ar) override
    {
        ar >> _inductor.inductance;
    }
};
