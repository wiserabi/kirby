#pragma once

class Rect
{
public:
    Rect(Vector3 position, Vector3 size, float rotation);
    ~Rect();

    void Update();
    void Render();

    void SetPosition(Vector3 pos);
    Vector3 GetPosition() { return position; }
    Vector3 GetSize() { return size; }
    Vector3 GetLT() { return Vector3(position.x - size.x / 2, position.y + size.y / 2, 0.0f); }
    Vector3 GetRB() { return Vector3(position.x + size.x / 2, position.y - size.y / 2, 0.0f); }
    float GetRotation() { return rotation; }

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

    Color color = Color(0.0f, 0.0f, 0.0f, 0.0f);

    BoundingBox* box = nullptr;
    bool bOpen = true;
};