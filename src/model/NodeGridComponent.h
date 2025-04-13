//
//  NodeGridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 12. 4. 2025..
//

#pragma once
#include "IGridComponent.h"

class NodeGridComponent : public IGridComponent
{
public:
    gui::Point centerPoint;

public:
    NodeGridComponent(const gui::Point &pt)
        : centerPoint(pt)
    {
    }

    void draw() const override
    {
        _shape.drawWire(td::ColorID::Yellow);
    }

    void getBoundingRect(gui::Rect &boundRect) override
    {
        boundRect = gui::Rect(gui::Circle(centerPoint, 1));
    }

    void load(arch::ArchiveIn &ar) override
    {
    }

    void save(arch::ArchiveOut &ar) const override
    {
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
        gui::Circle circle(centerPoint, 1);
        _shape.createCircle(circle);
    }

    void translate(const gui::Point &delta) override
    {
        centerPoint.x += delta.x;
        centerPoint.y += delta.y;
        updateShape();
    }

    void updateShape() override
    {
        gui::Circle circle(centerPoint, 1);
        _shape.updateCircleNodes(circle);
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
};
