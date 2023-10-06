#pragma once

#include <iostream>
#include <vector>

struct Line {
    float prevCreateTime;
    Vector2 p1, p2, p3;
    //x, y, t
    std::vector<Vector3> water_drops;
};

class BezierCurves {
public:
    BezierCurves() : lines_() {}

    void Add(Vector2 p1, Vector2 p2, Vector2 p3);

    void Set(Vector2 p1, Vector2 p2, Vector2 p3, int index);

    void Remove(int index);

    float GetCurveLength(Line line);

    void CreateWaterdrop(int line_index);

    void Update();

    std::vector<Line> GetLines() { return lines_; };

    void Clear() {
        if (!lines_.empty()) {
            lines_.clear();
        }
    }

private:
    std::vector<Line> lines_;

    Vector2 GetBezierPoint(Vector2 p1, Vector2 p2, Vector2 p3, float t) {
        float u = 1 - t;
        float tt = t * t;
        float uu = u * u;

        Vector2 point;
        point.x = uu * p1.x + 2 * u * t * p2.x + tt * p3.x;
        point.y = uu * p1.y + 2 * u * t * p2.y + tt * p3.y;

        return point;
    }
};
