#pragma once

class Rect
{
public:
    Rect(Vector3 position, Vector3 size, float rotation);
    ~Rect();

    void Update();
    void Render();

    void SetPosition(Vector3 pos);

    void SetColor(Color color);
    void UpdateColor();

    void Move();

    void GUI();
    string MakeLabel(string preFix);
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

    Color color = Color(0.5, 0.5, 0.5, 0.7);

    BoundingBox* box = nullptr;
    bool bOpen = true;
};