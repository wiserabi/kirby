#include "stdafx.h"
#include "Geomatries/TextureRect.h"
#include "Geomatries/Rect.h"
#include "Level.h"
#include "ReadCoordinate.h"
#include "Enemy.h"
#include "EnemyInfo.h"

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

	slopeRange.push_back({ 10092.0f , 10232.0f });//up 45degree
	slopeRange.push_back({ 10232.0f , 10502.0f });//up 27degree
	slopeRange.push_back({ 10590.0f , 10854.0f });//down 27degree
	slopeRange.push_back({ 10854.0f , 10994.0f });//down 45degree
	slopeRange.push_back({ 11468.0f , 11746.0f });//up 27degree
	slopeRange.push_back({ 12384.0f , 12790.0f });//down 27degree

	enemyInfo = new EnemyInfo();
	//6034 700
	enemies.push_back(new Enemy({ position.x + 100.0f, position.y + 500.0f, 0 }, { 128.0f, 128.0f, 0.0f }, "sparky", enemyInfo));
	enemies.push_back(new Enemy({ position.x + 100.0f, position.y + 100.0f, 0 }, { 128.0f, 128.0f, 0.0f }, "waddledee", enemyInfo));
	enemies.push_back(new Enemy({ position.x + 100.0f, position.y + 800.0f, 0 }, { 128.0f, 128.0f, 0.0f }, "waddledoo", enemyInfo));
}

Level::~Level()
{
	vector<pair<float, float>>().swap(slopeRange);
	SAFE_DELETE(enemyInfo);
	vector<Enemy*>().swap(enemies);
	SAFE_DELETE(levelMap);
	vector<Rect*>().swap(rects);
}

void Level::Init()
{
}

void Level::Destroy()
{
}

void Level::Update()
{
	//check enemy death
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->CheckDeath()) {
			enemies.erase(enemies.begin() + i);
			i--;
		}
		else {
			//set enemy collision to default
			enemies[i]->SetHitGround(false);
			enemies[i]->SetHitLeft(false);
			enemies[i]->SetHitRight(false);
			//set kirby position for enemy to chase
			enemies[i]->SetKirbyPos(kirbyPosition);
		}
	}

	levelMap->Update();
	for (int i = 0; i < rects.size(); i++) {
		rects[i]->Update();
	}
	EnemyCollisions();
	//update all the enemies
	for (class Enemy* enemy : enemies) {
		enemy->SetKirbyPos(kirbyPosition);
		enemy->Update();
	}
}

void Level::Render()
{
	levelMap->Render();
	//update all the enemies
	for (class Enemy* enemy : enemies) {
		enemy->Render();
	}
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

void Level::SetKirbyPosition(Vector3 kirbyPosition)
{
	this->kirbyPosition = kirbyPosition;
}

void Level::EnemyCollisions()
{
	//assume there are enemies in the world
	for (size_t i = 0; i < enemies.size(); i++)
	{
		Rect* enemyRect = enemies[i]->GetRect();
		EnemyCollisionLevel(enemyRect, i);
	}
}

void Level::EnemyCollisionLevel(Rect* enemyRect, int enemyIdx)
{
	BoundingBox* enemyBox = nullptr;
	if (enemyRect) {
		enemyBox = enemyRect->GetBox();
	}
	//iterate level rects
	for (int i = 0; i < rects.size(); i++) {
		if (enemyBox && BoundingBox::OBB(enemyBox, rects[i]->GetBox())) {
			float rotation = rects[i]->GetRotation();
			Vector3 enemyPos = enemies[enemyIdx]->GetPosition();
			//collision with slope
			if (rotation > 0.1f) {//upper
				int idx = CheckSlopeRange(enemyPos.x);
				if (idx > -1) {
					SetEnemyPosForSlope(idx, enemyPos, rotation, enemies[enemyIdx]);
				}
			}
			else if (rotation < -0.1f) {//down
				int idx = CheckSlopeRange(enemyPos.x);
				if (idx > -1) {
					SetEnemyPosForSlope(idx, enemyPos, rotation, enemies[enemyIdx]);
				}
			}
			else {
				FixEnemyPosition(rects[i], enemyIdx);
			}
		}
	}
}

void Level::IntersectRect(Rect* enemyRect, Rect* levelRect, pair<Vector3, Vector3>& intersection) {
	Vector3 enemyLT = enemyRect->GetLT();
	Vector3 worldLT = levelRect->GetLT();
	Vector3 enemyRB = enemyRect->GetRB();
	Vector3 worldRB = levelRect->GetRB();


	float left = max(enemyLT.x, worldLT.x);
	float top = min(enemyLT.y, worldLT.y);
	float right = min(enemyRB.x, worldRB.x);
	float bottom = max(enemyRB.y, worldRB.y);
	if (left >= right || top <= bottom) {
		intersection = make_pair(Values::ZeroVec3, Values::ZeroVec3);
		return;
	}
	intersection = make_pair(Vector3(left, top, 0), Vector3(right, bottom, 0));
	return;
}

void Level::FixEnemyPosition(Rect* levelRect, int idx)
{
	pair<Vector3, Vector3> intersection;
	IntersectRect(enemies[idx]->GetRect(), levelRect, intersection);

	Vector3 enemyPos = enemies[idx]->GetPosition();
	Vector3 enemySize = enemies[idx]->GetRect()->GetSize();

	Vector3 enemyLT = enemies[idx]->GetLT();
	Vector3 enemyRB = enemies[idx]->GetRB();

	Vector3 worldRectPos = levelRect->GetPosition();
	Vector3 worldLT = levelRect->GetLT();
	Vector3 worldRB = levelRect->GetRB();

	Vector3 tmp = enemyPos - worldRectPos;
	if (intersection.first == Values::ZeroVec3 &&
		intersection.second == Values::ZeroVec3) {
		return;
	}
	// Determine the collision direction based on the intersection size
	if (intersection.first.x - intersection.second.x >
		intersection.second.y - intersection.first.y) {
		if (enemyLT.x < worldLT.x) {
			// Right collision
			enemyPos.x = worldLT.x - enemySize.x / 2 + 1;
			enemies[idx]->SetPosition(enemyPos);
			enemies[idx]->SetHitRight(true);
		}
		else {
			// Left collision
			enemyPos.x = worldRB.x + enemySize.x / 2 - 1;
			enemies[idx]->SetPosition(enemyPos);
			enemies[idx]->SetHitLeft(true);
		}
	}
	else
	{
		if (enemyLT.y > worldLT.y) {
			// Down collision
			enemyPos.y = worldLT.y + enemySize.y / 2 - 1;
			enemies[idx]->SetPosition(enemyPos);
			enemies[idx]->SetHitGround(true);
		}
		else {
			// Up collision
			enemyPos.y = worldRB.y - enemySize.y / 2;
			enemies[idx]->SetPosition(enemyPos);
		}
	}
}

vector<class Enemy*> Level::GetEnemies()
{
	return enemies;
}

int Level::CheckSlopeRange(float positionX)
{
	for (int i = 0; i < slopeRange.size(); i++) {
		if (slopeRange[i].first <= positionX && positionX <= slopeRange[i].second) {
			return i;
		}
	}

	return -1;
}

void Level::SetEnemyPosForSlope(int idx, Vector3 enemyPos, float rotation, class Enemy* enemy)
{
	switch (idx)
	{
	case 0:
		enemyPos.y = floor + (enemyPos.x - slopeRange[0].first);
		break;
	case 1:
		enemyPos.y = floor2 + (enemyPos.x - slopeRange[1].first) * tan27;
		break;
	case 2:
		enemyPos.y = floor2 + (slopeRange[2].second - enemyPos.x) * tan27;
		break;
	case 3:
		enemyPos.y = floor + (slopeRange[3].second - enemyPos.x);
		break;
	case 4:
		enemyPos.y = floor + (enemyPos.x - slopeRange[4].first) * tan27;
		break;
	case 5:
		enemyPos.y = floor + (slopeRange[5].second - enemyPos.x) * tan27;
		break;
	default:
		break;
	}
	enemy->SetHitGround(true);
	enemy->SetPosition(enemyPos);
}