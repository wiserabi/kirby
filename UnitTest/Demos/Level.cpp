#include "stdafx.h"
#include "Geomatries/TextureRect.h"
#include "Geomatries/Rect.h"
#include "Level.h"
#include "ReadCoordinate.h"

Level::Level(Vector3 pos, wstring pngName)
{
	this->position = pos;
	size = Vector3(WinMaxHeight * 6 + 60, WinMaxHeight, 0);
	levelMap = new TextureRect(position, size, 0.0f,
		TexturePath + L"backGround/" + pngName + L".png");
	FileReader fr;
	wstring txtName = pngName + L".txt";
	vector<Vector3> tmp = fr.ReadFile(CoordPath + String::ToString(txtName));
	for (int i = 0; i < tmp.size() / 2; i++) {
		Vector3 pos = tmp[2 * i];
		Vector3 size = tmp[2 * i + 1];
		float angle_in_degrees = (float)pos.z;
		pos.z = 0.0f;
		size.z = 0.0f;
		//cout << pos.z << "\n";
		float angle_in_radians = angle_in_degrees * (3.141592 / 180.0f);
		rects.push_back(new Rect(pos, size, angle_in_radians));
	}
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
	for (int i = 0; i < rects.size(); i++) {
		rects[i]->Update();
	}
	
}

void Level::Render()
{
	levelMap->Render();
	for (int i = 0; i < rects.size(); i++) {
		rects[i]->Render();
	}
}

void Level::PostRender()
{
}

void Level::GUI()
{
}

Vector3 Level::GetLT()
{
	return levelMap->GetLT();
}

Vector3 Level::GetRB()
{
	return levelMap->GetRB();
}
