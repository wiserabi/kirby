#pragma once

class TextureRect
{
public:
    TextureRect(Vector3 position, Vector3 size, float rotation, wstring path);
    TextureRect(Vector3 position, Vector3 size, float rotation);
    ~TextureRect();

    void Update();
    void Render();

    void SetShader(wstring shaderPath);
    void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }

    void MapVertexBuffer();
    void UnmapVertexBuffer();
    Vector3 GetPosition();
    void SetPosition(Vector3 position);
    Vector3 GetLT() { return Vector3(position.x - size.x / 2, position.y + size.y / 2, 0.0f); }
    Vector3 GetRB() { return Vector3(position.x + size.x / 2, position.y - size.y / 2, 0.0f); }

protected:
    vector<VertexTexture> vertices;
    VertexBuffer* vb = nullptr;

    vector<uint> indices;
    IndexBuffer* ib = nullptr;

    InputLayout* il = nullptr;

    VertexShader* vs = nullptr;
    PixelShader* ps = nullptr;

    Matrix world;
    Matrix S, R, T;

    WorldBuffer* wb = nullptr;

    Vector3 position;
    Vector3 size;
    float rotation;

    ID3D11ShaderResourceView* srv = nullptr;
    D3D11_MAPPED_SUBRESOURCE subResource;
    string path;
};