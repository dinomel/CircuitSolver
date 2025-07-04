//
//  NodeGridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 12. 4. 2025..
//

#pragma once
#include <gui/Shape.h>
#include "IGridComponent.h"
#include "../property/DefaultSettings.h"

// global parameters
extern DefaultSettings g_defaultSettings;

class NodeGridComponent : public IGridComponent
{
    const double _nodeRadius = 1.5;
    gui::Shape _nodeShape;

public:
    gui::Point centerPoint;
    bool isFloating = false;

public:
    NodeGridComponent(const gui::Point &pt)
        : centerPoint(pt)
    {
    }

    void updateIsFloating(bool floating)
    {
        isFloating = floating;
    }

    void draw(bool isSelected) const override
    {
        td::ColorID nodeColor = isFloating ? td::ColorID::Red : isSelected ? g_defaultSettings.getSelectedComponentColor()
                                                                           : g_defaultSettings.getComponentColor();
        _nodeShape.drawFillAndWire(nodeColor, nodeColor);
    }

    void getBoundingRect(gui::Rect &boundRect) override
    {
        boundRect = gui::Rect(gui::Circle(centerPoint, _nodeRadius));
    }

    Type getType() const override
    {
        return Type::Node;
    }

    bool canBeSelected(const gui::Point &pt) const override
    {
        return distanceToPointSquared(pt) < 144;
    }

    void init() override
    {
        gui::Circle circle(centerPoint, _nodeRadius);
        _nodeShape.createCircle(circle);
    }

    void translate(const gui::Point &delta) override
    {
        centerPoint.x += delta.x;
        centerPoint.y += delta.y;
        updateShape();
    }

    void updateShape() override
    {
        gui::Circle circle(centerPoint, _nodeRadius);
        _nodeShape.updateCircleNodes(circle);
    }

    double distanceToPointSquared(const gui::Point &pt) const override
    {
        return (centerPoint.x - pt.x) * (centerPoint.x - pt.x) + (centerPoint.y - pt.y) * (centerPoint.y - pt.y);
    }

    void snapToGrid() override
    {
        centerPoint = getClosestGridPoint(centerPoint);
        updateShape();
    }

    void release() override
    {
        delete this;
    }

    void getValues(gui::PropertyValues &propValues) const override
    {
    }

    void setValues(gui::PropertyValues &propValues) override
    {
    }

    void setValue(td::UINT4 key, gui::PropertyValues &propValues) override
    {
    }

    void save(arch::ArchiveOut &ar) const override
    {
    }

    void load(arch::ArchiveIn &ar) override
    {
    }
};
