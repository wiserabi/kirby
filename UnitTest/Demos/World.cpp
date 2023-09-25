#include "stdafx.h"
#include "World.h"
#include "Level.h"
#include "Geomatries/TextureRect.h"
#include "Geomatries/Rect.h"
#include "ReadCoordinate.h"

World::World()
{
	worldPos = Vector3(1000, 800, 0);
	worldSize = Vector3(2000, 2000, 0);
	worldMap = new TextureRect(worldPos, worldSize, 0.0f,
		TexturePath + L"backGround/world1.png");
	rect = new Rect(worldPos - Vector3(950, 100, 0), Vector3(100, 600, 0), 0.0f);
	rect->SetColor(Values::Magenta);
	FileReader fr;
	vector<Vector3> tmp = fr.ReadFile(CoordPath + "world1.txt");
	for (int i = 0; i < tmp.size() / 2; i++) {
		Vector3 pos = tmp[2 * i];
		Vector3 size = tmp[2 * i + 1];
		rects.push_back(new Rect(pos, size, pos.z));
	}
	
}

World::~World()
{
	rects.clear();
}

void World::Render()
{
	worldMap->Render();
	rect->Render();
	for (int i = 0; i < 10; i++) {
		rects[i]->Render();
	}
}

void World::PostRender()
{
}

void World::GUI()
{
	//rect->GUI();
}

void World::SetColor(int index, Color color)
{
	rects[index]->SetColor(color);
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
	//rect->Update();
	for (int i = 0; i < 10; i++) {
		rects[i]->Update();
	}
}
