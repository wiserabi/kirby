#pragma once

class Rect
{
public:
    Rect(Vector3 position, Vector3 size, float rotation);
    ~Rect();

    void Update();
    void Render();

    void SetColor(Color color);
    void UpdateColor();

    void Move();

    BoundingBox* GetBox() { return box; }

private:
    vector<VertexColor> vertices;
    VertexBuffer* vb = nullptr;

    vector<uint> indices;
    IndexBuffer* ib = nullptr;

    InputLayout* il = nullptr;

    VertexShader* vs = nullptr;
    PixelShader* ps = nullptr;

    Matrix world, S, R, T;

    Vector3 position;
    Vector3 size;
    float rotation;

    WorldBuffer* wb = nullptr;

    Color color = Values::Magenta;

    BoundingBox* box = nullptr;
};