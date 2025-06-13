//
//  GroundGridComponent.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 17. 4. 2025..
//

#pragma once
#include "GridComponent.h"
#include "../core/Component.h"

class GroundGridComponent : public GridComponent
{

public:
    GroundGridComponent(NodeGridComponent *startNode, NodeGridComponent *endNode)
        : GridComponent(startNode, endNode, 12, 36)
    {
    }

    Component *getComponent() override
    {
        return nullptr;
    }

    Type getType() const override
    {
        return Type::Ground;
    }

    void draw(bool isSelected) const override
    {
        for (const gui::Shape &shape : _componentShapes)
        {
            shape.drawWire(isSelected ? td::ColorID::SpringGreen : td::ColorID::Yellow);
        }
        _wiresShape.drawWire(isSelected ? td::ColorID::SpringGreen : td::ColorID::Yellow);
        startNode->draw(isSelected);
    }

    void init() override
    {
        gui::Point points[] = {
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
            getStartPoint(),
            getEndPoint(),
        };
        _wiresShape.createLines(&points[0], 2);
        _componentShapes[0].createLines(&points[0], 6);
    }

    void updateShape() override
    {
        double x_A = getStartPoint().x;
        double y_A = getStartPoint().y;
        double x_B = getEndPoint().x;
        double y_B = getEndPoint().y;

        double theta = std::atan2(y_B - y_A, x_B - x_A);
        double sTheta = sin(theta);
        double cTheta = cos(theta);

        double x_D = x_B - _width * cTheta;
        double y_D = y_B - _width * sTheta;
        double x_C = (x_D + x_B) / 2;
        double y_C = (y_D + y_B) / 2;

        gui::Point point_D(x_D, y_D);

        gui::Point wirePoints[] = {
            getStartPoint(),
            point_D,
        };
        _wiresShape.createLines(&wirePoints[0], 2);

        double halfHeight = _height / 2;
        double eightHeight = _height / 8;
        double middleLength = 5 * _height / 16;

        double x_G = x_D + halfHeight * sTheta;
        double y_G = y_D - halfHeight * cTheta;
        double x_F = x_D - halfHeight * sTheta;
        double y_F = y_D + halfHeight * cTheta;

        double x_I = x_B + eightHeight * sTheta;
        double y_I = y_B - eightHeight * cTheta;
        double x_H = x_B - eightHeight * sTheta;
        double y_H = y_B + eightHeight * cTheta;

        double x_J = x_C + middleLength * sTheta;
        double y_J = y_C - middleLength * cTheta;
        double x_K = x_C - middleLength * sTheta;
        double y_K = y_C + middleLength * cTheta;

        gui::Point point_G(x_G, y_G);
        gui::Point point_F(x_F, y_F);
        gui::Point point_H(x_H, y_H);
        gui::Point point_I(x_I, y_I);
        gui::Point point_J(x_J, y_J);
        gui::Point point_K(x_K, y_K);

        gui::Point points[] = {
            point_F,
            point_G,
            point_I,
            point_H,
            point_J,
            point_K,
        };
        _componentShapes[0].createLines(&points[0], 6);
    }
};
