struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
};

struct PixelInput
{
    float4 position : SV_POSITION0;
	float2 uv : TEXCOORD0;
};

cbuffer WorldBuffer : register(b0)
{
	matrix _world;
}

cbuffer VPBuffer : register(b1)
{
	matrix _view;
	matrix _proj;
}

cbuffer BlurBuffer : register(b0)
{
	float2 _imageSize;
	int _blurCount;
}

PixelInput VS(VertexInput input)
{
	PixelInput output;
	
	output.position = mul(input.position, _world);
	output.position = mul(output.position, _view);
	output.position = mul(output.position, _proj);
	
	output.uv = input.uv;
	return output;
}

Texture2D _sourceTex : register(t0);
SamplerState _samp : register(s0);

float4 PS(PixelInput input) : SV_Target
{
	float4 color = _sourceTex.Sample(_samp, input.uv);
	
	float2 arr[8] =
	{
		float2(-1, +1), float2(+0, +1), float2(+1, +1),
		float2(-1, +0), /* 기준정점 */   float2(+1, +0),
		float2(-1, -1), float2(+0, -1), float2(+1, -1)
	};
	
	for (int blur = 1; blur < _blurCount; blur++)
	{
		for (int i = 0; i < 8; i++)
		{
			float x = arr[i].x * (float) blur / _imageSize.x;
			float y = arr[i].y * (float) blur / _imageSize.y;
			
			float2 uv = input.uv + float2(x, y);
			color += _sourceTex.Sample(_samp, uv);
		}
	}
	
	int blurInterations = _blurCount - 1;
	int offSetCount = 8;
	int totalSamples = blurInterations * offSetCount + 1;
	
	return color /= totalSamples;
}