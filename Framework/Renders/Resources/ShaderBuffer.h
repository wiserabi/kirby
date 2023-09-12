// GPU에서 사용하는 데이터를 CPU에서 생성하고 수정할 수 있도록 버퍼를 제공하는 역할
#pragma once
#include "Framework.h"

class ShaderBuffer
{
public:
    void SetVSBuffer(uint slot)
    {
        MapData();
        DC->VSSetConstantBuffers(slot, 1, &buffer);
    }
    void SetPSBuffer(uint slot)
    {
        MapData();
        DC->PSSetConstantBuffers(slot, 1, &buffer);
    }

protected:
    ShaderBuffer(void* data, uint dataSize)
        : data(data), dataSize(dataSize)
    {
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = dataSize;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        HRESULT hr = DEVICE->CreateBuffer(&desc, nullptr, &buffer);
        CHECK(hr);
    }

private:
    void MapData()
    {
        D3D11_MAPPED_SUBRESOURCE subResource;
        HRESULT hr = DC->Map
        (
            buffer,
            0,
            D3D11_MAP_WRITE_DISCARD,
            0,
            &subResource
        );
        CHECK(hr);

        memcpy(subResource.pData, data, dataSize);
        DC->Unmap(buffer, 0);
    }

private:
    D3D11_BUFFER_DESC desc = { 0 };
    ID3D11Buffer* buffer = nullptr;

    void* data = nullptr;
    uint dataSize = 0;
};