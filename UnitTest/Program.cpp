#include "stdafx.h"
#include "Program.h"

#include "Systems/Window.h"

#include "Demos/01_RectDemo.h"
#include "Demos/02_TextureDemo.h"
#include "Demos/03_ShadedDemo.h"
#include "Demos/04_RenderTargetDemo.h"
#include "Demos/05_CollisionDemo.h"
#include "Demos/06_AnimationDemo.h"

void Program::Init()
{
	Camera::Create();

	//Push(new RectDemo);
	//Push(new TextureDemo);
	//Push(new ShadedDemo);
	//Push(new RenderTargetDemo);
	//Push(new CollisionDemo);
	Push(new AnimationDemo);
}

void Program::Destroy()
{
	Camera::Delete();
	for (IObject* obj : objs)
	{
		obj->Destroy();
		SAFE_DELETE(obj);
	}


}

void Program::Update()
{
	for (IObject* obj : objs)
		obj->Update();

	Camera::Get()->Update();
}

void Program::Render()
{
	for (IObject* obj : objs)
		obj->Render();
	Camera::Get()->Render();
}

void Program::PostRender()
{
	for (IObject* obj : objs)
		obj->PostRender();
}

void Program::GUI()
{
	for (IObject* obj : objs)
	{
		obj->GUI();
	}
}

void Program::Push(IObject* obj)
{
	objs.push_back(obj);
	obj->Init();
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR param, int command)
{
	srand((UINT)time(NULL));

	DXDesc desc;
	desc.AppName = L"D2DGame";
	desc.instance = instance;
	desc.handle = NULL;
	desc.width = WinMaxWidth;
	desc.height = WinMaxHeight;

	Program* program = new Program();
	Window* window = new Window(desc);
	WPARAM wParam = window->Run(program);

	SAFE_DELETE(window);
	SAFE_DELETE(program);

	return wParam;

}
