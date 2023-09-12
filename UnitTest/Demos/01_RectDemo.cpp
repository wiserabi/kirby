#include "stdafx.h"
#include "01_RectDemo.h"

#include "Geomatries/Rect.h"

void RectDemo::Init()
{
    r = new Rect(Vector3(700, 400, 0), Vector3(500, 500, 0), 0);
}

void RectDemo::Destroy()
{
    SAFE_DELETE(r);
}

void RectDemo::Update()
{
    r->Update();
}

void RectDemo::Render()
{
    r->Render();
}

void RectDemo::PostRender()
{
}

void RectDemo::GUI()
{
}
