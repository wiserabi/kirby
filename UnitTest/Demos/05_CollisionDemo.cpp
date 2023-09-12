#include "stdafx.h"
#include "05_CollisionDemo.h"

#include "Geomatries/Rect.h"

void CollisionDemo::Init()
{
    r1 = new Rect({ 100,100,0 }, { 100,100,0 }, 30.0f);
    r2 = new Rect({ 300,300,0 }, { 100,100,0 }, 0.0f);
}

void CollisionDemo::Destroy()
{
    SAFE_DELETE(r2);
    SAFE_DELETE(r1);
}

void CollisionDemo::Update()
{
    r1->Move();

    r1->Update();
    r2->Update();

    if (BoundingBox::OBB(r1->GetBox(), r2->GetBox()))
        r2->SetColor(Values::Blue);
    else
        r2->SetColor(Values::Magenta);
}

void CollisionDemo::Render()
{
    r1->Render();
    r2->Render();
}

void CollisionDemo::PostRender()
{
}

void CollisionDemo::GUI()
{
}
