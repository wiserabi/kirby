#include "stdafx.h"
#include "Geomatries/TextureRect.h"
#include "Level.h"

Level::Level(Vector3 pos, wstring pngName)
{
	this->position = pos;
	size = Vector3(WinMaxHeight * 6 + 60, WinMaxHeight + 10, 0);
	levelMap = new TextureRect(position, size, 0.0f,
		TexturePath + L"backGround/" + pngName);
	
}

Level::~Level()
{
	SAFE_DELETE(levelMap);
}

void Level::Init()
{
}

void Level::Destroy()
{
}

void Level::Update()
{
	levelMap->Update();
}

void Level::Render()
{
	levelMap->Render();
}

void Level::PostRender()
{
}

void Level::GUI()
{
}
