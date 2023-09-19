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

cbuffer WorldBuffer : register(b0)  // 0 ~ 127
{
    matrix _world;
}

cbuffer VPBuffer : register(b1)
{
    matrix _view;
    matrix _projection;
}

cbuffer ProgressBuffer : register(b2)
{
    float _percent;
    uint _type;
}

PixelInput VS(VertexInput input)
{
    PixelInput output;

    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.uv = input.uv;
    return output;
}

Texture2D _sourceTex : register(t0);
SamplerState _samp : register(s0);

// 0 = NONE
// 1 = LEFT 2 RIGHT
// 3 = RIGHT 2 LEFT
// 4 = UP 2 DOWN
// 5 = DOWN 2 UP
float4 PS(PixelInput input) : SV_Target
{
    float4 color = _sourceTex.Sample(_samp, (float2) input.uv);
    float4 zero = float4(0, 0, 0, 0);

    if (_type == 1)
    {
        if (input.uv.x <= _percent)
            return color;
        else
            return zero;
    }
    else if (_type == 2)
    {
        if (input.uv.x >= _percent)
            return color;
        else
            return zero;
    }
    else if (_type == 3)
    {
        if (input.uv.y <= _percent)
            return color;
        else
            return zero;
    }
    else if (_type == 4)
    {
        if (1 - input.uv.y <= _percent)
            return color;
        else
            return zero;
    }

    return color;
}