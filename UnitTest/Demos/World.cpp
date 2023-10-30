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
	guiRect = new Rect(worldPos - Vector3(950, 100, 0), Vector3(100, 600, 0), 0.0f);
	guiRect->SetColor(Values::Magenta);
	FileReader fr;
	vector<Vector3> tmp = fr.ReadFile(CoordPath + "world1.txt");
	for (int i = 0; i < tmp.size() / 2; i++) {
		Vector3 pos = tmp[2 * i];
		Vector3 size = tmp[2 * i + 1];
		rects.push_back(new Rect(pos, size, pos.z));
	}
	Vector3 levelPos = Vector3(1000.0f, 500.0f, 0.0f);
	for (int i = 0; i < 3; i++) {
		levelPos.x += WinMaxHeight * 7;
		levels.push_back(new Level(levelPos, levelPng[i]));
		levels[i]->CreateRandomEnemies();
	}
	levelPos.x += WinMaxHeight * 7;
	levels.push_back(new Level(levelPos, Vector3(WinMaxWidth, WinMaxWidth * 1.48f, 0.0f), 
		levelPng[3]));
}

World::~World()
{
	for (auto& ptr : levels) {
		delete ptr;
	}
	levels.clear();
	for (auto& ptr : rects) {
		delete ptr;
	}
	rects.clear();

	SAFE_DELETE(worldMap);
}

void World::Render()
{
	worldMap->Render();

	for (int i = 0; i < rects.size(); i++) {
		rects[i]->Render();
	}
	for (int i = 0; i < LEVELNUM; i++) {
		levels[i]->Render();
	}
	if (enableGui) {
		guiRect->Render();
	}

}

void World::PostRender()
{
}

void World::GUI()
{
	if (enableGui) {
		guiRect->GUI();
	}
}

Vector3 World::GetLT()
{
	return worldMap->GetLT();
}

Vector3 World::GetRB()
{
	return worldMap->GetRB();
}

void World::SetColor(int index, Color color)
{
	rects[index]->SetColor(color);
}

void World::SetKirbyPos(Vector3 kirbyPos)
{
	this->kirbyPos = kirbyPos;
	for (size_t i = 0; i < levels.size(); i++)
	{
		levels[i]->SetKirbyPosition(kirbyPos);
	}
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

	for (int i = 0; i < 10; i++) {
		rects[i]->Update();
	}
	if (kirbyLocation == 1) {//level1
		levels[0]->Update();
	}
	else if (kirbyLocation == 2) {//level2
		levels[1]->Update();
	}
	else if (kirbyLocation == 3) {//level3
		levels[2]->Update();
	}
	else if (kirbyLocation == 4) {//boss
		levels[3]->Update();
	}
	if (Keyboard::Get()->Down(VK_F2)) {
		if (enableGui) {
			enableGui = false;
		}
		else {
			enableGui = true;
		}
	}
	if (enableGui) {
		guiRect->Update();

	}
}
