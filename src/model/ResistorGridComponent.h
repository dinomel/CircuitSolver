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

class ResistorGridComponent : public GridComponent
{
protected:
    Resistor _resistor;

public:
    ResistorGridComponent(double resistance, NodeGridComponent *startNode, NodeGridComponent *endNode)
        : GridComponent(startNode, endNode, 40, 16), _resistor(resistance)
    {
    }

    virtual Component *getComponent()
    {
        return &_resistor;
    }

    virtual Type getType() const
    {
        return Type::Resistor;
    }

    void initProperties(gui::Properties *properties) const
    {

        // if (createGroup)
        {
            auto &prop = properties->push_back();
            prop.setGroup("Parameters");
        }

        td::Variant valR(_resistor.resistance);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Resistance, "Resistance", valR);
        }

        valR = td::Variant(_resistor.impedance);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Impedance, "Impedance", valR);
        }

        // group
        {
            auto &prop = properties->push_back();
            prop.setGroup("NodesPosition");
        }

        td::Point<int> startCoordinate = getStartCoordinate();
        td::Point<int> endCoordinate = getEndCoordinate();

        td::Variant val(startCoordinate.x);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::X1, "StartNodeX", val);
        }

        val = td::Variant(startCoordinate.y);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Y1, "StartNodeY", val);
        }

        val = td::Variant(endCoordinate.x);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::X2, "EndNodeX", val);
        }

        val = td::Variant(endCoordinate.y);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Y2, "EndNodeY", val);
        }
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
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
        };
        _shape.createLines(&points[0], 12);
    }

    virtual void load(arch::ArchiveIn &ar)
    {
        //        GridComponent::load(ar);
        //        ar >> _startPoint;
    }

    virtual void save(arch::ArchiveOut &ar) const
    {
        //        GridComponent::save(ar);
        //        ar << _startPoint;
    }

    virtual void getValues(gui::PropertyValues &propValues) const
    {

        td::Variant valR(_resistor.resistance);
        propValues.setValueByKey((td::UINT4)PropID::Resistance, valR);

        td::Variant valX(_resistor.impedance);
        propValues.setValueByKey((td::UINT4)PropID::Impedance, valX);

        td::Point<int> startCoordinate = getStartCoordinate();
        td::Point<int> endCoordinate = getEndCoordinate();

        td::Variant x1(startCoordinate.x);
        propValues.setValueByKey((td::UINT4)PropID::X1, x1);

        td::Variant y1(startCoordinate.y);
        propValues.setValueByKey((td::UINT4)PropID::Y1, y1);

        td::Variant x2(endCoordinate.x);
        propValues.setValueByKey((td::UINT4)PropID::X2, x2);

        td::Variant y2(endCoordinate.y);
        propValues.setValueByKey((td::UINT4)PropID::Y2, y2);

        GridComponent::getValues(propValues);
    }

    virtual void setValues(gui::PropertyValues &propValues)
    {
        //                gui::Rect rBefore(_rect);
        //                if (isWithLine())
        //                    rBefore.inflate(_lineWidth+IGridComponent::refreshOffset);
        //
        //                propValues.setRectBeforeUpdate(rBefore);

        td::Variant resistance = propValues.getValueByKey((td::UINT4)PropID::Resistance);
        resistance.getValue(_resistor.resistance);

        td::Variant impedance = propValues.getValueByKey((td::UINT4)PropID::Impedance);
        impedance.getValue(_resistor.impedance);

        td::Variant x1 = propValues.getValueByKey((td::UINT4)PropID::X1);
        int startNodeX = 0;
        x1.getValue(startNodeX);
        startNode->centerPoint.x = startNodeX * gridSize;

        td::Variant y1 = propValues.getValueByKey((td::UINT4)PropID::Y1);
        int startNodeY = 0;
        y1.getValue(startNodeY);
        startNode->centerPoint.y = startNodeY * gridSize;

        td::Variant x2 = propValues.getValueByKey((td::UINT4)PropID::X2);
        int endNodeX = 0;
        x2.getValue(endNodeX);
        endNode->centerPoint.x = endNodeX * gridSize;

        td::Variant y2 = propValues.getValueByKey((td::UINT4)PropID::Y2);
        int endNodeY = 0;
        y2.getValue(endNodeY);
        endNode->centerPoint.y = endNodeY * gridSize;

        //                td::Variant varW = propValues.getValueByKey((td::UINT4)PropID::Width);
        //                gui::CoordType w;
        //                varW.getValue(w);
        //                _rect.setWidth(w);
        //
        //                td::Variant varH = propValues.getValueByKey((td::UINT4)PropID::Height);
        //                gui::CoordType h;
        //                varH.getValue(h);
        //                _rect.setHeight(h);
        //                gui::Rect rectAfter(_rect);

        GridComponent::setValues(propValues);
        updateShape();
        //                gui::Rect rAfter(_rect);

        //                if (isWithLine())
        //                    rAfter.inflate(_lineWidth+IGridComponent::refreshOffset);
        //                propValues.setRectAfterUpdate(rAfter);
        //
        //                if (getType() == IGridComponent::Type::Rect)
        //                {
        //                    if (rBefore != rAfter)
        //                        _shape.updateRectNodes(_rect);
        //                }
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
            point_G,
            point_I,
            point_I,
            point_H,
            point_H,
            point_F,
            point_E,
            getEndPoint(),
        };
        _shape.createLines(&points[0], 12);
    }
};
