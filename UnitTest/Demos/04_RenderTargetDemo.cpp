#include "stdafx.h"
#include "04_RenderTargetDemo.h"

#include "Geomatries/Rect.h"
#include "Geomatries/TextureRect.h"
#include "Utilities/RenderTexture.h"

void RenderTargetDemo::Init()
{
    rt = new RenderTexture;

    Vector3 size = Vector3(100, 100, 0);
    Vector3 trPos = Vector3(WinMaxWidth - (WinMaxWidth / 4), WinMaxHeight - (WinMaxHeight / 4), 0);
    Vector3 trSize = Vector3(WinMaxWidth / 2, WinMaxHeight / 2, 0);
    float rot = 0;

    r1 = new Rect(Vector3(200, 200, 0), size, rot);
    r2 = new Rect(Vector3(500, 400, 0), size, rot);
    r3 = new Rect(Vector3(700, 100, 0), size, rot);

    tr1 = new TextureRect(Vector3(350, 350, 0), Vector3(400, 400, 0), rot, TexturePath + L"pen.png");
    tr2 = new TextureRect(trPos, trSize, rot);
    tr2->SetShader(ShaderPath + L"RTV.hlsl");
}

void RenderTargetDemo::Destroy()
{
    SAFE_DELETE(tr2);
    SAFE_DELETE(tr1);
    SAFE_DELETE(r3);
    SAFE_DELETE(r2);
    SAFE_DELETE(r1);
    SAFE_DELETE(rt);
}

void RenderTargetDemo::Update()
{
    r1->Update();
    r2->Update();
    r3->Update();

    tr1->Update();
    tr2->Update();
}

void RenderTargetDemo::Render()
{
    r1->Render();
    r2->Render();
    r3->Render();

    tr1->Render();

    tr2->SetSRV(rt->GetSRV());
    tr2->Render();
}

void RenderTargetDemo::PostRender()
{
    Graphics::Get()->InitViewport();
    rt->RenderToTexture();
    {
        r1->Render();
        r2->Render();
        r3->Render();

        //tr1->Render();
    }
}

void RenderTargetDemo::GUI()
{
}
