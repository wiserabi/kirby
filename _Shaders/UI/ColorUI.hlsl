struct VertexInput
{
    float4 position : POSITION0;       // ���� ��ġ "position"�̶�� ������ "POSITION0" ��Ʈ���� ��ġ�� ���� ������ �����Ѵ�
    float4 color : COLOR0;             // ���� ����
};

struct PixelInput
{
    float4 position : SV_POSITION0;    // �ȼ� ��ġ     �ȼ��� ��ġ�� ��Ÿ���� �ý��� ���� �ǹ�
    float4 color : COLOR0;             // �ȼ� ����
};

// cbuffer : ��� ���� ��������
// ��� ���� �������� b0�� �Ҵ�� ���� ����� �����ϴ� ��� ����
cbuffer WorldBuffer : register(b0)  // 0 ~ 127
{
    matrix _world;
}

// ��� ���� �������� b1�� �Ҵ�� ��, �������� ����� �����ϴ� ��� ����
cbuffer VPBuffer : register(b1)
{
    matrix _view;
    matrix _projection;
}

// ���� ���̴� �Լ�
// �Է����� VertexInput�� �ް� PixelInput ����ü�� ��ȯ
PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    output.position = mul(input.position, _world);          // ���� ��ġ�� ���� ����� ����
    output.position = mul(output.position, _view);          // ����� �� ����� ����
    output.position = mul(output.position, _projection);    // ����� �������� ����� ����
    
    output.color = input.color;
    return output;
}

// �ȼ� ���̴� �Լ�
// �Է����� PixelInput ����ü�� �ް�, float4 ������ �ȼ� ������ ��ȯ
float4 PS(PixelInput input) : SV_Target
{
    return input.color;
}

/*
Semantic : ����ƽ
- HLSL ���̴����� �������� �ǹ̸� �����ϴµ� ���
- ���̴��� �Է� �����Ϳ� ��� �����͸� �ùٸ��� �ؼ��ϰ� ó���� �� �ֵ��� ����
- ���� �̸� �ڿ� ':'��ȣ�� �Բ� ����
- �ý��� �� ����ƽ�� 'SV_'���λ�� �����ϸ� Direct3D���� ���ǵ� Ư���� �ǹ̸� ����
- �ý��� �� ����ƽ�� ���̴� �������� ���� �����͸� �����ϴµ� ���

slot
- GPU���� ����ϴ� ��� ����, �ؽ�ó, ���÷� ���� �ڿ����� �ĺ��ϴ� �ε���
- �� ������ ������ ��ȣ�� ������, �ش� �ڿ��� ������ ���ҿ� ���� �ٸ��� �Ҵ��
- register��� Ű���带 ����Ͽ� ����
- �� �ڿ��� � ���Կ� �Ҵ�� ������ ��������� ������ �� ����
- �ֿ� ����
    1) ��� ���� ����
    - ��� �����͸� �����ϴµ� ���, VS�� PS���� ������ �� �ִ�
    - ��� ���� ������ register(b#)�� ����Ͽ� ����

    2) �ؽ�ó ����
    - �̹��� �����͸� �����ϴµ� ���
    - �ؽ�ó ���� ������ register(t#)�� ����Ͽ� ����

    3) ���÷� ����
    - �ؽ�ó�� ���ø� �ϴ� �� ���
    - ���÷� ������ register(s#)�� ����Ͽ� ����

cbuffer = Constance Buffer : ��� ����
- ���̴����� ����ϴ� ���� ������ �����ϴµ� ���
- �� ��� ���� �������Ϳ��� �� ���� ��� ���۸� �Ҵ��� �� ����
- ��� ���� ���ο��� ���� ���� ������ ������ �� �ִ�.
*/