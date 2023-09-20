#include "stdafx.h"
#include "World.h"
#include "Level.h"
#include "Geomatries/TextureRect.h"

World::World()
{
	worldPos = Vector3(1000, 800, 0);
	worldSize = Vector3(2000, 2000, 0);
	worldMap = new TextureRect(worldPos, worldSize, 0.0f,
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
