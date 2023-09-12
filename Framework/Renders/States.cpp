#include "Framework.h"
#include "States.h"

#define ZERO(p) ZeroMemory(&p, sizeof(p))

D3D11_RASTERIZER_DESC States::rsDesc;
D3D11_DEPTH_STENCIL_DESC States::dsDesc;
D3D11_SAMPLER_DESC States::spDesc;
D3D11_BLEND_DESC States::blDesc;


void States::Create()
{
	CreateRasterizerDesc();
	CreateDepthStencilDesc();
	CreateSamplerDesc();
	CreateBlendDesc();
}

void States::GetRasterizerDesc(D3D11_RASTERIZER_DESC* desc)
{
	memcpy(desc, &rsDesc, sizeof(rsDesc));
}

void States::GetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC* desc)
{
	memcpy(desc, &dsDesc, sizeof(dsDesc));
}

void States::GetSamplerDesc(D3D11_SAMPLER_DESC* desc)
{
	memcpy(desc, &spDesc, sizeof(spDesc));
}

void States::GetBlendDesc(D3D11_BLEND_DESC* desc)
{
	memcpy(desc, &blDesc, sizeof(blDesc));
}

void States::CreateRasterizer(D3D11_RASTERIZER_DESC* desc, ID3D11RasterizerState** state)
{
	HRESULT hr = DEVICE->CreateRasterizerState(desc, state);
	CHECK(hr);
}

void States::CreateDepthStencil(D3D11_DEPTH_STENCIL_DESC* desc, ID3D11DepthStencilState** state)
{
	HRESULT hr = DEVICE->CreateDepthStencilState(desc, state);
	CHECK(hr);
}

void States::CreateSampler(D3D11_SAMPLER_DESC* desc, ID3D11SamplerState** state)
{
	HRESULT hr = DEVICE->CreateSamplerState(desc, state);
	CHECK(hr);
}

void States::CreateBlend(D3D11_BLEND_DESC* desc, ID3D11BlendState** state)
{
	HRESULT hr = DEVICE->CreateBlendState(desc, state);
	CHECK(hr);
}



void States::CreateRasterizerDesc()
{
	ZERO(rsDesc);
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FrontCounterClockwise = false;
	rsDesc.DepthBias = 0;
	rsDesc.DepthBiasClamp = 0;
	rsDesc.SlopeScaledDepthBias = 0;
	//rsDesc.DepthClipEnable;
	rsDesc.ScissorEnable = false;
	rsDesc.MultisampleEnable = false;
	rsDesc.AntialiasedLineEnable = false;
}

void States::CreateDepthStencilDesc()
{
	ZERO(dsDesc);
	
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xff;
	dsDesc.StencilWriteMask = 0xff;
	


	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	

	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

}

void States::CreateSamplerDesc()
{
	ZERO(spDesc);

	spDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	spDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	spDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	spDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	spDesc.MaxAnisotropy = 1;
	spDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	spDesc.MinLOD = FLT_MIN;
	spDesc.MaxLOD = FLT_MAX;
}

void States::CreateBlendDesc()
{
	ZERO(blDesc);
	
	blDesc.AlphaToCoverageEnable = false;
	blDesc.IndependentBlendEnable = false;

	blDesc.RenderTarget[0].BlendEnable = false;
	blDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	blDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	blDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}
