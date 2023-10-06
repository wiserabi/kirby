#include "stdafx.h"
#include "BezierCurves.h"

void BezierCurves::Add(Vector2 p1, Vector2 p2, Vector2 p3)
{
    Line line = { 0.0f, p1, p2, p3 };
    lines_.push_back(line);
}

void BezierCurves::Set(Vector2 p1, Vector2 p2, Vector2 p3, int index)
{
    if (index >= 0 && index < lines_.size()) {
        lines_[index].p1 = p1;
        lines_[index].p2 = p2;
        lines_[index].p3 = p3;
    }
}

void BezierCurves::Remove(int index)
{
    if (index >= 0 && index < lines_.size()) {
        lines_.erase(lines_.begin() + index);
    }
}

float BezierCurves::GetCurveLength(Line line) {
    float length = 0.0f;
    Vector2 prev_point = line.p1;
    for (int i = 1; i <= 100; ++i) {
        float t = static_cast<float>(i) / 100;
        Vector2 point = GetBezierPoint(line.p1, line.p2, line.p3, t);
        length += sqrt(pow(point.x - prev_point.x, 2) + pow(point.y - prev_point.y, 2));
        prev_point = point;
    }
    return length;
}

void BezierCurves::CreateWaterdrop(int line_index) {
    if (line_index >= 0 && line_index < lines_.size()) {
        Line& line = lines_[line_index];
        //save the time created for next creation
        line.prevCreateTime = Time::Get()->Running();
        Vector2 point = GetBezierPoint(line.p1, line.p2, line.p3, 1);
        line.water_drops.push_back(Vector3(point.x, point.y, 1.0f));
    }
}

void BezierCurves::Update() {
    //change position of line elements according to kirby pos
    for (int i = 0; i < lines_.size(); i++) {
        Line& line = lines_[i];
        //create new water drop with random time
        float randInterval = (rand() % 50) * 0.01f + 0.1f;
        if (Time::Get()->Running() - line.prevCreateTime > randInterval) {
            CreateWaterdrop(i);
        }
        for (int j = 0; j < line.water_drops.size(); ++j) {
            Vector3& drop = line.water_drops[j];

            drop.z -= 0.04f;
            Vector2 nextPoint = GetBezierPoint(line.p1, line.p2, line.p3, drop.z);
            drop.x = nextPoint.x;
            drop.y = nextPoint.y;

            // Remove the water drop t < 0.0f
            if (drop.z < 0.0f) {
                line.water_drops.erase(line.water_drops.begin() + j);
                --j;
            }
        }
    }
    lines_;
}

