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
- ���� ������ ����(����) ��ǥ�迡�� ���� ��ǥ��� ��ȯ�ϴ� ���� �ǹ�
- ���� ��ġ, ȸ�� �� ũ�⸦ ���� (SRT)

2. View
- ī�޶��� ��ġ, ���� �� �þ߸� ��Ÿ���� ���
- ���� ��ǥ�迡�� ī�޶� �ٶ󺸴� ������ ��Ÿ��
- ī�޶��� ��ġ�� ������ ����

3. Projection
- 3D ������ 2D ��ũ�� ��ǥ�� ��ȯ�ϴ� ���� �ǹ�
- �þ߰�, ��Ⱦ��, ũ���� ���(Clipping Planes)���� ����Ͽ� ����
*/