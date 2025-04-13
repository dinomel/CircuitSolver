//
//  CoordinatePoint.h
//  CircuitSolver
//
//  Created by Dino Melunovic on 13. 4. 2025..
//

#pragma once
class CoordinatePoint
{
public:
    td::Point<int> point;

    CoordinatePoint(int x, int y)
    {
        point.x = x;
        point.y = y;
    }

    CoordinatePoint(const td::Point<int> &p) : point(p) {}

    CoordinatePoint(const gui::Point &p) : point(int(p.x), int(p.y)) {}

    int getX() const { return point.x; }
    int getY() const { return point.y; }

    bool operator<(const CoordinatePoint &other) const
    {
        return std::tie(point.x, point.y) < std::tie(other.point.x, other.point.y);
    }
};
