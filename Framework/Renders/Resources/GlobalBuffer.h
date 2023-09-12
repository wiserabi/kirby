#pragma once
#include "Framework.h"

class WorldBuffer : public ShaderBuffer
{
public:
    WorldBuffer() : ShaderBuffer(&data, sizeof(Data))
    {
        D3DXMatrixIdentity(&data.world);
    }

    void SetWorld(Matrix world)
    {
        D3DXMatrixTranspose(&data.world, &world);
    }

public:
    struct Data
    {
        Matrix world;
    };

private:
    Data data;
};

class VPBuffer : public ShaderBuffer
{
public:
    VPBuffer() : ShaderBuffer(&data, sizeof(Data))
    {
        D3DXMatrixIdentity(&data.view);
        D3DXMatrixIdentity(&data.projection);
    }

    void SetView(Matrix view)
    {
        D3DXMatrixTranspose(&data.view, &view);
    }
    void SetProjection(Matrix projection)
    {
        D3DXMatrixTranspose(&data.projection, &projection);
    }

public:
    struct Data
    {
        Matrix view;
        Matrix projection;
    };

private:
    Data data;
};

/*
1. World
- 모델의 정점을 지역(로컬) 좌표계에서 월드 좌표계로 변환하는 것을 의미
- 모델의 위치, 회전 및 크기를 조정 (SRT)

2. View
- 카메라의 위치, 방향 및 시야를 나타내는 행렬
- 월드 좌표계에서 카메라가 바라보는 방향을 나타냄
- 카메라의 위치와 방향을 변경

3. Projection
- 3D 공간을 2D 스크린 좌표로 변환하는 것을 의미
- 시야각, 종횡비, 크리핑 평면(Clipping Planes)등을 고려하여 수행
*/