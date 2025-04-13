//
//  GridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 8. 4. 2025..
//

#pragma once
#include "IGridComponent.h"
#include "NodeGridComponent.h"
#include "../core/Component.h"

extern const int gridSize;

class GridComponent : public IGridComponent
{

protected:
    double _width;
    double _height;

public:
    NodeGridComponent *startNode;
    NodeGridComponent *endNode;

public:
    GridComponent(NodeGridComponent *startNode, NodeGridComponent *endNode, double width, double height)
        : _width(width), _height(height), startNode(startNode), endNode(endNode)
    {
    }

    virtual ~GridComponent()
    {
    }

    enum class PropID : td::UINT4
    {
        X1 = 200,
        Y1,
        X2,
        Y2,
        Resistance,
        Impedance,
        Inductance,
        Capacitance,
    };

    virtual void getBoundingRect(gui::Rect &boundRect)
    {
        boundRect = gui::Rect(startNode->centerPoint, endNode->centerPoint);
    }

    virtual bool hasLength() const
    {
        return !(startNode->centerPoint == endNode->centerPoint);
    }

    virtual gui::Point getStartPoint() const
    {
        return startNode->centerPoint;
    }

    virtual gui::Point getEndPoint() const
    {
        return endNode->centerPoint;
    }

    virtual td::Point<int> getStartCoordinate() const
    {
        return {
            int(startNode->centerPoint.x / gridSize),
            int(startNode->centerPoint.y / gridSize),
        };
    }

    virtual td::Point<int> getEndCoordinate() const
    {
        return {
            int(endNode->centerPoint.x / gridSize),
            int(endNode->centerPoint.y / gridSize),
        };
    }

    virtual Component *getComponent() = 0;

    virtual void draw() const
    {
        _shape.drawWire(td::ColorID::Yellow);
        startNode->draw();
        endNode->draw();
    }

    virtual void translate(const gui::Point &delta)
    {
        startNode->translate(delta);
        endNode->translate(delta);
        updateShape();
    }

    virtual void snapToGrid()
    {
        startNode->snapToGrid();
        endNode->snapToGrid();
        updateShape();
    }

    virtual void updateEndPoint(const gui::Point &newEndPoint)
    {
        endNode->centerPoint = newEndPoint;
        endNode->updateShape();
        updateShape();
    }

    virtual void updateStartPoint(const gui::Point &newStartPoint)
    {
        startNode->centerPoint = newStartPoint;
        startNode->updateShape();
        updateShape();
    }

    virtual double distanceToPointSquared(const gui::Point &pt) const
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

    virtual bool canBeSelected(const gui::Point &pt) const
    {
        return distanceToPointSquared(pt) < 144;
    }

    virtual void load(arch::ArchiveIn &ar)
    {
        ////        td::BYTE attr = 0 ; //(td::BYTE) _attribs;
        ////        td::BYTE lnPattern = 0; //(td::BYTE) _linePattern;
        //        arch::EnumLoader<gui::Shape::Attribs> attr(_attribs, gui::Shape::Attribs::LineAndFill, gui::Shape::Attribs::LineAndFill);
        //        arch::EnumLoader<td::LinePattern> lnPattern(_linePattern, td::LinePattern::NA, td::LinePattern::Solid);
        //        ar >> _lineWidth >> attr >> _fillColor >> _lineColor >> lnPattern;
        ////        _attribs = (gui::Shape::Attribs) attr;
        ////        _linePattern = (td::LinePattern) lnPattern;
    }

    virtual void save(arch::ArchiveOut &ar) const
    {
        ////        td::BYTE attr = (td::BYTE) _attribs;
        ////        td::BYTE lnPattern = (td::BYTE) _linePattern;
        //        arch::EnumSaver<gui::Shape::Attribs> attr(_attribs);
        //        arch::EnumSaver<td::LinePattern> lnPattern(_linePattern);
        //        ar << _lineWidth << attr << _fillColor << _lineColor << lnPattern;
    }

    virtual void release()
    {
        startNode->release();
        endNode->release();
        delete this;
    }

    virtual void setValue(td::UINT4 key, gui::PropertyValues &propValues)
    {
    }

    virtual void initProperties(gui::Properties *properties) const = 0;

    virtual void initNodesProperties(gui::Properties *properties) const
    {
        // group
        {
            auto &prop = properties->push_back();
            prop.setGroup("Start Node Position");
        }

        td::Point<int> startCoordinate = getStartCoordinate();

        td::Variant val(startCoordinate.x);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::X1, "X", val);
        }

        val = td::Variant(startCoordinate.y);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Y1, "Y", val);
        }

        // group
        {
            auto &prop = properties->push_back();
            prop.setGroup("End Node Position");
        }

        td::Point<int> endCoordinate = getEndCoordinate();

        val = td::Variant(endCoordinate.x);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::X2, "X", val);
        }

        val = td::Variant(endCoordinate.y);
        {
            auto &prop = properties->push_back();
            prop.set((td::UINT4)PropID::Y2, "Y", val);
        }
    }

    virtual void getNodesValues(gui::PropertyValues &propValues) const
    {
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
    }

    virtual void setNodesValues(gui::PropertyValues &propValues)
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
    }
};
