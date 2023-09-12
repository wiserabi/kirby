#pragma once

class VertexBuffer
{
public:
    ~VertexBuffer();

    template<typename T>
    void Create(const vector<T>& vertices, const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT);

    ID3D11Buffer* GetResource() { return buffer; }

    uint GetStride() { return stride; }
    uint GetOffset() { return offset; }
    uint GetCount() { return count; }

    void SetIA();

private:
    ID3D11Buffer* buffer;

    uint stride = 0;
    uint offset = 0;
    uint count = 0;
};

template<typename T>
inline void VertexBuffer::Create(const vector<T>& vertices, const D3D11_USAGE& usage)
{
    stride = sizeof(T);
    count = vertices.size();

    D3D11_BUFFER_DESC desc;
    ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

    desc.Usage = usage;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.ByteWidth = stride * count;

    switch (usage)
    {
        case D3D11_USAGE_DEFAULT:
        case D3D11_USAGE_IMMUTABLE:
            break;

        case D3D11_USAGE_DYNAMIC:
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            break;

        case D3D11_USAGE_STAGING:
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
            break;
    }

    D3D11_SUBRESOURCE_DATA subData;
    ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));

    subData.pSysMem = vertices.data();

    HRESULT hr = DEVICE->CreateBuffer(&desc, &subData, &buffer);
    CHECK(hr);
}

/*						GPU				CPU
D3D11_USAGE_DEFAULT		읽기, 쓰기		접근 불가
D3D11_USAGE_IMMUTABLE	읽기				접근 불가
D3D11_USAGE_DYNAMIC		읽기				쓰기(Map사용) (느림)
D3D11_USAGE_STAGING		GPU 메모리에서 CPU 메모리로 복사를 허용 (CopyResource) (엄청 느림)
*/