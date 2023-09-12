#include "Framework.h"
#include "Rect.h"

Rect::Rect(Vector3 position, Vector3 size, float rotation)
    : position(position), size(size), rotation(rotation)
{
    vertices.assign(4, VertexColor());

    vertices[0].position = Vector3(-0.5f, -0.5f, 0.0f);
    vertices[1].position = Vector3(+0.5f, +0.5f, 0.0f);
    vertices[2].position = Vector3(+0.5f, -0.5f, 0.0f);
    vertices[3].position = Vector3(-0.5f, +0.5f, 0.0f);

    for (auto& v : vertices)
        v.color = color;

    vb = new VertexBuffer;
    vb->Create(vertices, D3D11_USAGE_DYNAMIC);

    indices = { 0,1,2,0,3,1 };
    ib = new IndexBuffer;
    ib->Create(indices, D3D11_USAGE_IMMUTABLE);

    vs = new VertexShader();
    vs->Create(ShaderPath + L"VertexColor.hlsl", "VS");

    ps = new PixelShader();
    ps->Create(ShaderPath + L"VertexColor.hlsl", "PS");

    il = new InputLayout;
    il->Create(VertexColor::descs, VertexColor::count, vs->GetBlob());

    wb = new WorldBuffer;

    box = new BoundingBox(position, size, rotation);
}

Rect::~Rect()
{
    SAFE_DELETE(box);
    SAFE_DELETE(wb);
    SAFE_DELETE(il);
    SAFE_DELETE(ps);
    SAFE_DELETE(vs);
    SAFE_DELETE(ib);
    SAFE_DELETE(vb);
}

void Rect::Update()
{
    D3DXMatrixScaling(&S, size.x, size.y, size.z);
    D3DXMatrixRotationZ(&R, rotation);
    D3DXMatrixTranslation(&T, position.x, position.y, position.z);

    world = S * R * T;
    wb->SetWorld(world);

    box->Update(position, size, rotation);
}

void Rect::Render()
{
    //IA
    vb->SetIA();
    ib->SetIA();
    il->SetIA();
    DC->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //VS
    vs->SetShader();
    wb->SetVSBuffer(0);

    //PS
    ps->SetShader();

    DC->DrawIndexed(ib->GetCount(), 0, 0);
}

void Rect::SetColor(Color color)
{
    this->color = color;
    UpdateColor();
}

void Rect::UpdateColor()
{
    D3D11_MAPPED_SUBRESOURCE subResource;
    DC->Map(vb->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    {
        vertices[0].color = color;
        vertices[1].color = color;

        memcpy(subResource.pData, vertices.data(), vb->GetCount() * vb->GetStride());
    }
    DC->Unmap(vb->GetResource(), 0);
}

void Rect::Move()
{
    auto* key = Keyboard::Get();

    if (key->Press('W'))
        position.y += 15;
    if (key->Press('S'))
        position.y -= 15;
    if (key->Press('D'))
        position.x += 15;
    if (key->Press('A'))
        position.x -= 15;
}
