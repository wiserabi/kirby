#pragma once
#include "TextureRect.h"

class ShadedBuffer : public ShaderBuffer
{
public:
    ShadedBuffer() : ShaderBuffer(&data, sizeof(data))
    {
        data.selection = 1;
    }

    int* GetSelectionPtr() { return &data.selection; }

    struct Data
    {
        int selection;
        Vector3 dummy;
    };

private:
    Data data;
};

class BlurBuffer : public ShaderBuffer
{
public:
    BlurBuffer() : ShaderBuffer(&data, sizeof(data))
    {
        data.textureSize = Values::ZeroVec2;
        data.blurCount = 1;
    }

    void SetTextureSize(Vector2 size)
    {
        data.textureSize = size;
    }

    int* GetBlurCountPtr() { return &data.blurCount; }

    struct Data
    {
        Vector2 textureSize;
        int blurCount;
        int dummy;
    };
private:
    Data data;
};

class ShadedTexture : public TextureRect
{
public:
    ShadedTexture(Vector3 position, Vector3 size, float rotation, wstring path);
    ~ShadedTexture();

    void Render();
    void GUI();

private:
    //ShadedBuffer* sb = nullptr;
    BlurBuffer* bb = nullptr;
};