struct VertexInput
{
	float4 position : POSITION0;
	float2 uv : TEXCOORD0;
	float2 uv2 : TEXCOORD1;
	float4 color : COLOR0;
	uint index : INDEX0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;
	float2 uv : TEXCOORD0;
	float2 uv2 : TEXCOORD1;
	float4 color : COLOR0;
	uint index : INDEX0;
};

cbuffer WorldBuffer : register(b0)
{
	matrix _world;
}

cbuffer VPBuffer : register(b1)
{
	matrix _view;
	matrix _projection;
}

cbuffer IndexNumBuffer : register(b0)
{
	uint _index;
}

PixelInput VS(VertexInput input)
{
	PixelInput output;

	output.position = mul(input.position, _world);
	output.position = mul(output.position, _view);
	output.position = mul(output.position, _projection);

	output.uv = input.uv;
	output.uv2 = input.uv2;
	output.color = input.color;
	output.index = input.index;

	return output;
}

Texture2D _sourceTex : register(t0);
SamplerState _samp : register(s0);

float4 PS(PixelInput input) : SV_Target
{
	float4 color = 0;

	if (input.uv.x > 0.0f || input.uv.y > 0.0f)
		color = _sourceTex.Sample(_samp, input.uv);
	else
		color = input.color;

	if (input.index == _index)
	{
		if (input.uv2.x < 0.05f || input.uv2.x > 0.95f || input.uv2.y < 0.05f || input.uv2.y > 0.95f)
			color = float4(1, 0, 0, 1);
	}

	return color;
}