//
//  GridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 8. 4. 2025..
//

#pragma once
#include <gui/Shape.h>
#include "IGridComponent.h"
#include "NodeGridComponent.h"
#include "CoordinatePoint.h"
#include "../core/Component.h"
#include <iostream>
#include "../property/DefaultSettings.h"

// global parameters
extern DefaultSettings g_defaultSettings;
extern const int gridSize;

class GridComponent : public IGridComponent
{

protected:
    gui::Shape _wiresShape;
    std::vector<gui::Shape> _componentShapes;
    double _width;
    double _height;
    //    std::complex<double> _current;
    //    std::complex<double> _voltage;
    //    std::complex<double> _power;
    double _current;
    double _voltage;
    double _power;

public:
    NodeGridComponent *startNode;
    NodeGridComponent *endNode;

public:
    GridComponent(NodeGridComponent *startNode, NodeGridComponent *endNode, double width, double height, int nShapes = 1)
        : _width(width), _height(height), startNode(startNode), endNode(endNode), _componentShapes(nShapes)
    {
    }

    enum class PropID : td::UINT4
    {
        X1 = 200,
        Y1,
        X2,
        Y2,
        Resistance,
        Reactance,
        Inductance,
        Capacitance,
        Voltage,
        MaxVoltage,
        PhaseAngle,
        Current,
        ResI,
        ResVd,
        ResP,
    };

    void getBoundingRect(gui::Rect &boundRect) override
    {
        boundRect = gui::Rect(startNode->centerPoint, endNode->centerPoint);
    }

    bool hasLength() const
    {
        return !(startNode->centerPoint == endNode->centerPoint);
    }

    gui::Point getStartPoint() const
    {
        return startNode->centerPoint;
    }

    gui::Point getEndPoint() const
    {
        return endNode->centerPoint;
    }

    CoordinatePoint getStartCoordinate() const
    {
        return {
            int(startNode->centerPoint.x / gridSize),
            int(startNode->centerPoint.y / gridSize),
        };
    }

    CoordinatePoint getEndCoordinate() const
    {
        return {
            int(endNode->centerPoint.x / gridSize),
            int(endNode->centerPoint.y / gridSize),
        };
    }

    virtual Component *getComponent() = 0;

    void init() override
    {
        gui::Point points[] = {
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
        };
        _wiresShape.createLines(&points[0], 4);
    }

    std::tuple<gui::Point, gui::Point, double> updateWiresShape()
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
        gui::Point point_D(x_D, y_D);
        gui::Point point_E(x_E, y_E);

        gui::Point points[] = {
            getStartPoint(),
            point_D,
            point_E,
            getEndPoint(),
        };
        _wiresShape.createLines(&points[0], 4);
        return {point_D, point_E, theta};
    }

    void draw(bool isSelected) const override
    {
        for (const gui::Shape &shape : _componentShapes)
        {
            shape.drawWire(isSelected ? g_defaultSettings.getSelectedComponentColor() : g_defaultSettings.getComponentColor());
        }
        _wiresShape.drawWire(isSelected ? g_defaultSettings.getSelectedComponentColor() : g_defaultSettings.getComponentColor());
        startNode->draw(isSelected);
        endNode->draw(isSelected);
    }

    void translate(const gui::Point &delta) override
    {
        startNode->translate(delta);
        endNode->translate(delta);
        updateShape();
    }

    void snapToGrid() override
    {
        startNode->snapToGrid();
        endNode->snapToGrid();
        updateShape();
    }

    void setCurrent(double current)
    {
        _current = current;
    }

    void setVoltage(double voltage)
    {
        _voltage = voltage;
    }

    void updateEndPoint(const gui::Point &newEndPoint)
    {
        endNode->centerPoint = newEndPoint;
        endNode->updateShape();
        updateShape();
    }

    void updateStartPoint(const gui::Point &newStartPoint)
    {
        startNode->centerPoint = newStartPoint;
        startNode->updateShape();
        updateShape();
    }

    double distanceToPointSquared(const gui::Point &pt) const override
    {
        double x0 = pt.x;
        double y0 = pt.y;
        double x1 = startNode->centerPoint.x;
        double y1 = startNode->centerPoint.y;
        double x2 = endNode->centerPoint.x;
        double y2 = endNode->centerPoint.y;

        double dx = x2 - x1;
        double dy = y2 - y1;

        double projected = ((x0 - x1) * dx + (y0 - y1) * dy) / (dx * dx + dy * dy);

        if (projected < 0)
        {
            return (x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1);
        }
        else if (projected > 1)
        {
            return (x0 - x2) * (x0 - x2) + (y0 - y2) * (y0 - y2);
        }
        else
        {
            double numeratorSqRoot = abs((dy * x0 - dx * y0 + (x2 * y1 - y2 * x1)));
            double denominator = dy * dy + dx * dx * 1;
            return numeratorSqRoot * numeratorSqRoot / denominator;
        }
    }

    bool canBeSelected(const gui::Point &pt) const override
    {
        return distanceToPointSquared(pt) < 144;
    }

    virtual void load(arch::ArchiveIn &ar) override
    {
        ////        td::BYTE attr = 0 ; //(td::BYTE) _attribs;
        ////        td::BYTE lnPattern = 0; //(td::BYTE) _linePattern;
        //        arch::EnumLoader<gui::Shape::Attribs> attr(_attribs, gui::Shape::Attribs::LineAndFill, gui::Shape::Attribs::LineAndFill);
        //        arch::EnumLoader<td::LinePattern> lnPattern(_linePattern, td::LinePattern::NA, td::LinePattern::Solid);
        //        ar >> _lineWidth >> attr >> _fillColor >> _lineColor >> lnPattern;
        ////        _attribs = (gui::Shape::Attribs) attr;
        ////        _linePattern = (td::LinePattern) lnPattern;
    }

    virtual void save(arch::ArchiveOut &ar) const override
    {
        ////        td::BYTE attr = (td::BYTE) _attribs;
        ////        td::BYTE lnPattern = (td::BYTE) _linePattern;
        //        arch::EnumSaver<gui::Shape::Attribs> attr(_attribs);
        //        arch::EnumSaver<td::LinePattern> lnPattern(_linePattern);
        //        ar << _lineWidth << attr << _fillColor << _lineColor << lnPattern;
    }

    void release() override
    {
        startNode->release();
        endNode->release();
        delete this;
    }

    void setValue(td::UINT4 key, gui::PropertyValues &propValues) override
    {
    }

    virtual void initProperties(gui::Properties *properties) const
    {
        // group
        {
            auto &prop = properties->push_back();
            prop.setGroup(gui::tr("startNodePosition"));
        }

        CoordinatePoint startCoordinate = getStartCoordinate();

        td::Variant val(startCoordinate.getX());
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::X1, "X", val);
        }

        val = td::Variant(startCoordinate.getY());
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Y1, "Y", val);
        }

        // group
        {
            auto &prop = properties->push_back();
            prop.setGroup(gui::tr("endNodePosition"));
        }

        CoordinatePoint endCoordinate = getEndCoordinate();

        val = td::Variant(endCoordinate.getX());
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::X2, "X", val);
        }

        val = td::Variant(endCoordinate.getY());
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Y2, "Y", val);
        }

        // group
        {
            auto &prop = properties->push_back();
            prop.setGroup(gui::tr("resultValues"));
        }

        val = td::Variant(_current);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::ResI, "I", val);
            prop.setReadOnly();
        }

        val = td::Variant(_voltage);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::ResVd, "Vd", val);
            prop.setReadOnly();
        }

        val = td::Variant(_power);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::ResP, "P", val);
            prop.setReadOnly();
        }
    }

    virtual void getValues(gui::PropertyValues &propValues) const override
    {
        CoordinatePoint startCoordinate = getStartCoordinate();
        CoordinatePoint endCoordinate = getEndCoordinate();

        td::Variant x1(startCoordinate.getX());
        propValues.setValueByKey((td::UINT4)PropID::X1, x1);

        td::Variant y1(startCoordinate.getY());
        propValues.setValueByKey((td::UINT4)PropID::Y1, y1);

        td::Variant x2(endCoordinate.getX());
        propValues.setValueByKey((td::UINT4)PropID::X2, x2);

        td::Variant y2(endCoordinate.getY());
        propValues.setValueByKey((td::UINT4)PropID::Y2, y2);

        td::Variant resI(_current);
        propValues.setValueByKey((td::UINT4)PropID::ResI, resI);

        td::Variant resVd(_voltage);
        propValues.setValueByKey((td::UINT4)PropID::ResVd, resVd);

        td::Variant resP(_power);
        propValues.setValueByKey((td::UINT4)PropID::ResP, resP);
    }

    virtual void setValues(gui::PropertyValues &propValues) override
    {
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

        propValues.setUpdateCanvas(true);
        startNode->updateShape();
        endNode->updateShape();
        updateShape();
    }
};
