#include "stdafx.h"
#include "02_TextureDemo.h"

#include "Geomatries/TextureRect.h"

void TextureDemo::Init()
{
    tr = new TextureRect(Vector3(300, 300, 0), Vector3(300, 300, 0), 0.0f, TexturePath + L"pen.png");
}

void TextureDemo::Destroy()
{
    SAFE_DELETE(tr);
}

void TextureDemo::Update()
{
    tr->Update();
}

void TextureDemo::Render()
{
    tr->Render();
}

void TextureDemo::PostRender()
{
}

void TextureDemo::GUI()
{
}
