//
//  ResistorGridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 8. 4. 2025..
//

#pragma once
#include "GridComponent.h"
#include "../core/Resistor.h"
#include "../core/Component.h"
#include <iostream>

class ResistorGridComponent : public GridComponent
{
protected:
    Resistor _resistor;

public:
    ResistorGridComponent(double resistance, NodeGridComponent *startNode, NodeGridComponent *endNode)
        : GridComponent(startNode, endNode, 40, 16), _resistor(resistance)
    {
    }

    Component *getComponent() override
    {
        return &_resistor;
    }

    Type getType() const override
    {
        return Type::Resistor;
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
        _componentShapes[0].createPolyLine(&points[0], 4);
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
        double x_G = x_D + halfHeight * sTheta;
        double y_G = y_D - halfHeight * cTheta;
        double x_F = x_D - halfHeight * sTheta;
        double y_F = y_D + halfHeight * cTheta;
        double x_I = x_E + halfHeight * sTheta;
        double y_I = y_E - halfHeight * cTheta;
        double x_H = x_E - halfHeight * sTheta;
        double y_H = y_E + halfHeight * cTheta;

        gui::Point point_G(x_G, y_G);
        gui::Point point_F(x_F, y_F);
        gui::Point point_H(x_H, y_H);
        gui::Point point_I(x_I, y_I);

        gui::Point points[] = {
            point_G,
            point_F,
            point_H,
            point_I,
        };
        _componentShapes[0].createPolygon(&points[0], 4);
    }

    void initProperties(gui::Properties *properties) const override
    {
        {
            auto &prop = properties->push_back();
            prop.setGroup(gui::tr("parameters"));
        }

        td::Variant valR(_resistor.resistance);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Resistance, gui::tr("resistance"), valR);
        }

        valR = td::Variant(_resistor.reactance);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Reactance, gui::tr("reactance"), valR);
        }

        GridComponent::initProperties(properties);
    }

    void getValues(gui::PropertyValues &propValues) const override
    {

        td::Variant valR(_resistor.resistance);
        propValues.setValueByKey((td::UINT4)PropID::Resistance, valR);

        td::Variant valX(_resistor.reactance);
        propValues.setValueByKey((td::UINT4)PropID::Reactance, valX);

        GridComponent::getValues(propValues);
    }

    void setValues(gui::PropertyValues &propValues) override
    {
        td::Variant resistance = propValues.getValueByKey((td::UINT4)PropID::Resistance);
        resistance.getValue(_resistor.resistance);

        td::Variant reactance = propValues.getValueByKey((td::UINT4)PropID::Reactance);
        reactance.getValue(_resistor.reactance);

        GridComponent::setValues(propValues);
    }

    void save(arch::ArchiveOut &ar) const override
    {
        GridComponent::save(ar);
        ar << _resistor.resistance << _resistor.reactance;
    }

    void load(arch::ArchiveIn &ar) override
    {
        ar >> _resistor.resistance >> _resistor.reactance;
    }
};
