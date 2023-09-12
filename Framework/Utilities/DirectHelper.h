#pragma once

class Values
{
public:
    const static float PI;

    const static Vector3 ZeroVec3;
    const static Vector3 OneVec3;
    const static Vector3 LeftVec;
    const static Vector3 RightVec;
    const static Vector3 UpVec;
    const static Vector3 DownVec;
    const static Vector3 FwdVec;
    const static Vector3 ScreenSize;

    const static Vector2 ZeroVec2;
    const static Vector2 OneVec2;
    const static Vector2 uvLT;
    const static Vector2 uvLB;
    const static Vector2 uvRT;
    const static Vector2 uvRB;

    const static Color Red;
    const static Color Greed;
    const static Color Blue;
    const static Color Magenta;
    const static Color Black;
    const static Color White;
    const static Color ClearColor;

    const static Matrix Identity;
};

namespace DXMath
{
    static Matrix Translation(const Vector3& position)
    {
        Matrix m;
        D3DXMatrixTranslation(&m, position.x, position.y, position.z);
        return m;
    }

    static Matrix Translation(const float& x, const float& y, const float& z)
    {
        return Translation(Vector3(x, y, z));
    }

    static Matrix Scaling(const Vector3& size)
    {
        Matrix m;
        D3DXMatrixScaling(&m, size.x, size.y, size.z);
        return m;
    }

    static Matrix Scaling(const float& x, const float& y, const float& z)
    {
        return Scaling(Vector3(x, y, z));
    }

    static Matrix Rotation(const float& rotation)
    {
        Matrix m;
        D3DXMatrixRotationZ(&m, rotation);
        return m;
    }
}