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

cbuffer ShadedBuffer : register(b0)
{
	int selection;
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
	float4 color;
	float4 resultColor;
	
	if (input.uv.x < 0.01f || input.uv.x > 0.99f || input.uv.y < 0.01f || input.uv.y > 0.99f)
		return float4(0, 1, 0, 1);
	
	if (input.uv.x < 0.51f && input.uv.x > 0.499f)
		return float4(0, 1, 0, 1);
	
	if (input.uv.y < 0.51f && input.uv.y > 0.499f)
		return float4(0, 1, 0, 1);
	
	// 좌상단
	if (input.uv.x < 0.5f && input.uv.y < 0.5f)
	{
		color = _sourceTex.Sample(_samp, float2(input.uv.x * 2.0f, input.uv.y * 2.0f));
		float3 temp;
		temp = dot(color.rgb, float3(0.299f, 0.587f, 0.144f));
		resultColor = float4(temp, color.a);
	}
	
	// 우상단
	if (input.uv.x >= 0.5f && input.uv.y < 0.5f)
	{
		color = _sourceTex.Sample(_samp, float2((input.uv.x - 0.5f) * 2.0f, input.uv.y * 2.0f));
        float3 gray = float3(0.393f, 0.769f, 0.189f);
		float3 temp = dot(color.rgb, gray);
		temp.r += 0.2f;
		temp.g += 0.1f;
		resultColor = float4(temp, color.a);
	}
	
	// 좌하단
	if (input.uv.x < 0.5f && input.uv.y >= 0.5f)
	{
		color = _sourceTex.Sample(_samp, float2(input.uv.x * 2.0f, (input.uv.y - 0.5f) * 2.0f));
		float3 negative = 1 - abs(color.rgb);
		resultColor = float4(negative, color.a);
	}
	
	// 우하단
	if (input.uv.x >= 0.5f && input.uv.y >= 0.5f)
	{
		color = _sourceTex.Sample(_samp, float2((input.uv.x - 0.5f) * 2.0f, (input.uv.y - 0.5f) * 2.0f));
		float3 value = float3(4.0f, 4.0f, 4.0f);
		float3 posterize = round(color.rgb * value) / value;
		resultColor = float4(posterize, color.a);
	}
	
	//if (selection == 1)
	//	return resultColor;
	//else if (selection == 2)
	//{
	//// AM
	//	float3 temp;
	//	temp = dot(color.rgb, float3(0.299f, 0.587f, 0.144f));
	//	resultColor = float4(temp, color.a);
	//}
	//else if (selection == 3)
	//{
	//// Sepia
	//	float3 gray = float3(0.393f, 0.769f, 0.189f);
	//	float3 temp = dot(color.rgb, gray);
	//	temp.r += 0.2f;
	//	temp.g += 0.1f;
	//	resultColor = float4(temp, color.a);
	//}
	//else if (selection == 4)
	//{
	//// Negative
	//	float3 negative = 1 - abs(color.rgb);
	//	resultColor = float4(negative, color.a);
	//}
	//else if (selection == 5)
	//{
	//// Posterize
	//	float3 value = float3(4.0f, 4.0f, 4.0f);
	//	float3 posterize = round(color.rgb * value) / value;
	//	resultColor = float4(posterize, color.a);
	//}
	
		return
resultColor;
}