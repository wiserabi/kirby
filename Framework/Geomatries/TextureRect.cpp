#include "Framework.h"
#include "TextureRect.h"

TextureRect::TextureRect(Vector3 position, Vector3 size, float rotation, wstring path)
    : position(position), size(size), rotation(rotation)
{
    vertices.assign(4, VertexTexture());

    vertices[0].position = Vector3(-0.5f, -0.5f, 0.0f);
    vertices[1].position = Vector3(+0.5f, +0.5f, 0.0f);
    vertices[2].position = Vector3(+0.5f, -0.5f, 0.0f);
    vertices[3].position = Vector3(-0.5f, +0.5f, 0.0f);
    
    vertices[0].uv = Vector2(0, 1);
    vertices[1].uv = Vector2(1, 0);
    vertices[2].uv = Vector2(1, 1);
    vertices[3].uv = Vector2(0, 0);
    
    //vertices[2].uv = Vector2(0, 1);
    //vertices[1].uv = Vector2(0, 0);
    //vertices[3].uv = Vector2(1, 0);
    //vertices[0].uv = Vector2(1, 1);

    vb = new VertexBuffer();
    vb->Create(vertices, D3D11_USAGE_DYNAMIC);

    indices = { 0,1,2,0,3,1};
    ib = new IndexBuffer();
    ib->Create(indices, D3D11_USAGE_IMMUTABLE);

    vs = new VertexShader();
    vs->Create(ShaderPath + L"VertexTexture.hlsl", "VS");

    ps = new PixelShader();
    ps->Create(ShaderPath + L"VertexTexture.hlsl", "PS");

    il = new InputLayout();
    il->Create(VertexTexture::descs, VertexTexture::count, vs->GetBlob());

    wb = new WorldBuffer();
    
    HRESULT hr = D3DX11CreateShaderResourceViewFromFile
    (
        DEVICE,
        path.c_str(),
        nullptr,
        nullptr,
        &srv,
        nullptr
    );
    CHECK(hr);
    this->path = String::ToString(path);
    SRV::Get()->AddSRV(this->path, srv);
}

TextureRect::TextureRect(Vector3 position, Vector3 size, float rotation)
    : position(position), size(size), rotation(rotation)
{
    vertices.assign(4, VertexTexture());

    vertices[0].position = Vector3(-0.5f, -0.5f, 0.0f);
    vertices[1].position = Vector3(+0.5f, +0.5f, 0.0f);
    vertices[2].position = Vector3(+0.5f, -0.5f, 0.0f);
    vertices[3].position = Vector3(-0.5f, +0.5f, 0.0f);

    vertices[1].uv = Vector2(1, 0);
    vertices[0].uv = Vector2(0, 1);
    vertices[2].uv = Vector2(1, 1);
    vertices[3].uv = Vector2(0, 0);

    vb = new VertexBuffer();
    vb->Create(vertices, D3D11_USAGE_DYNAMIC);

    indices = { 0,1,2,0,3,1 };
    ib = new IndexBuffer();
    ib->Create(indices, D3D11_USAGE_IMMUTABLE);

    vs = new VertexShader();
    vs->Create(ShaderPath + L"VertexTexture.hlsl", "VS");

    ps = new PixelShader();
    ps->Create(ShaderPath + L"VertexTexture.hlsl", "PS");

    il = new InputLayout();
    il->Create(VertexTexture::descs, VertexTexture::count, vs->GetBlob());

    wb = new WorldBuffer();
}

TextureRect::~TextureRect()
{
    //SAFE_RELEASE(srv);
    SAFE_DELETE(wb);
    SAFE_DELETE(il);
    SAFE_DELETE(ps);
    SAFE_DELETE(vs);
    SAFE_DELETE(ib);
    SAFE_DELETE(vb);
}

void TextureRect::Update()
{
    D3DXMatrixTranslation(&T, position.x, position.y, position.z);
    D3DXMatrixScaling(&S, size.x, size.y, size.z);
    D3DXMatrixRotationZ(&R, rotation);

    world = S * R * T;
    wb->SetWorld(world);
}

void TextureRect::Render()
{
    //srv = SRV::Get()->GetSRV(this->path);
    vb->SetIA();
    ib->SetIA();
    il->SetIA();
    DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    vs->SetShader();
    wb->SetVSBuffer(0);

    ps->SetShader();
    
    DC->PSSetShaderResources(0, 1, &srv);
    DC->DrawIndexed(ib->GetCount(), 0, 0);
}

void TextureRect::SetShader(wstring shaderPath)
{
    vs->Clear();
    ps->Clear();

    vs->Create(shaderPath, "VS");
    ps->Create(shaderPath, "PS");
}

void TextureRect::MapVertexBuffer()
{
    DC->Map(vb->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
}

void TextureRect::UnmapVertexBuffer()
{
    memcpy(subResource.pData, vertices.data(), sizeof(vertices[0]) * vertices.size());
    DC->Unmap(vb->GetResource(), 0);
}

Vector3 TextureRect::GetPosition()
{
    return Vector3();
}

void TextureRect::SetPosition(Vector3 position)
{
    this->position = position;
}
