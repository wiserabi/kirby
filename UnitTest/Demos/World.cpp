#include "stdafx.h"
#include "World.h"
#include "Level.h"
#include "Geomatries/TextureRect.h"

World::World()
{
	worldMap = new TextureRect(Vector3(300, 800, 0), Vector3(2000, 2000, 0), 0.0f,
		TexturePath + L"backGround/world1.png");

}

void World::Render()
{
	worldMap->Render();

}

void World::PostRender()
{
}

void World::GUI()
{
}

void World::Init()
{
}

void World::Destroy()
{
}

void World::Update()
{
	worldMap->Update();
}
