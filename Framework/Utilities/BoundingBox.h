#pragma once

enum
{
    x = 0,
    y
};

struct RectEdge
{
    Vector3 LT;
    Vector3 LB;
    Vector3 RT;
    Vector3 RB;
};

struct AxisData
{
    Vector3 centorPos;
    Vector3 axisDir[2];
    float axisLen[2];
};

class BoundingBox
{
public:
    BoundingBox(Vector3 position, Vector3 size, float rotation);
    ~BoundingBox();

    void Update(Vector3 position, Vector3 size, float rotation);

    static bool AABB(BoundingBox* a, BoundingBox* b);
    static bool OBB(BoundingBox* a, BoundingBox* b);

private:
    void UpdateCollisionData();

private:
    vector<VertexColor> vertices;

    Vector3 position;
    Vector3 size;
    float rotation;

    Matrix world;

    Color color = Values::Red;

    RectEdge* edge = nullptr;
    AxisData* data = nullptr;
};