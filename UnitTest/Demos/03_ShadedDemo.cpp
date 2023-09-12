#include "stdafx.h"
#include "03_ShadedDemo.h"

#include "Geomatries/ShadedTexture.h"

void ShadedDemo::Init()
{
    st = new ShadedTexture(Vector3(300, 300, 0), Vector3(400, 400, 0), 0.0f, TexturePath + L"pen.png");
}

void ShadedDemo::Destroy()
{
    SAFE_DELETE(st);
}

void ShadedDemo::Update()
{
    st->Update();
}

void ShadedDemo::Render()
{
    st->Render();
}

void ShadedDemo::PostRender()
{
}

void ShadedDemo::GUI()
{
    st->GUI();
}
