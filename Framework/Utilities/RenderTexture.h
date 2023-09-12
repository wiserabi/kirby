#pragma once

class RenderTexture
{
public:
    RenderTexture();
    ~RenderTexture();

    void RenderToTexture();
    ID3D11ShaderResourceView* GetSRV() { return srv; }

private:
    ID3D11Texture2D* texture = nullptr;
    ID3D11RenderTargetView* rtv = nullptr;
    ID3D11ShaderResourceView* srv = nullptr;
};