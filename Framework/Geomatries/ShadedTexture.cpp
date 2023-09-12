#include "Framework.h"
#include "ShadedTexture.h"

ShadedTexture::ShadedTexture(Vector3 position, Vector3 size, float rotation, wstring path)
    : TextureRect(position, size, rotation, path)
{
    //sb = new ShadedBuffer;
    //SetShader(ShaderPath + L"TextureFilter.hlsl");

    bb = new BlurBuffer();
    bb->SetTextureSize({ size.x, size.y });
    SetShader(ShaderPath + L"TextureBlur.hlsl");
}

ShadedTexture::~ShadedTexture()
{
    //SAFE_DELETE(sb);
    SAFE_DELETE(bb);
}

void ShadedTexture::Render()
{
    //sb->SetPSBuffer(0);
    bb->SetPSBuffer(0);
    __super::Render();
}

void ShadedTexture::GUI()
{
    using namespace ImGui;
    //Begin("Filter");
    //{
    //    RadioButton("Normal", sb->GetSelectionPtr(), 1);
    //    RadioButton("AM", sb->GetSelectionPtr(), 2);
    //    RadioButton("Sepia", sb->GetSelectionPtr(), 3);
    //    RadioButton("Negative", sb->GetSelectionPtr(), 4);
    //    RadioButton("Posterize", sb->GetSelectionPtr(), 5);
    //}
    //End();

    Begin("Blur");
    {
        ImGui::SliderInt("Blur Count", bb->GetBlurCountPtr(), 1, 100);
    }
    End();
}
