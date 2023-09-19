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
};