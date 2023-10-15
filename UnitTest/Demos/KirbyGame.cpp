#include "stdafx.h"
#include "KirbyGame.h"

#include "Geomatries/AnimationRect.h"
#include "KirbyCharacter.h"
#include "World.h"
#include "UI/HUD.h"
#include "Geomatries/Rect.h"
#include "Enemy.h"
#include "EnemyInfo.h"

void KirbyGame::Init()
{
	kirby = new KirbyCharacter({ 720, 360, 1.0 }, { 128, 128, 1 });
	Sounds::Get()->AddSound("Vegetable-Valley.mp3",
		SoundPath + L"Vegetable-Valley.mp3", true);
	Sounds::Get()->Play("Vegetable-Valley.mp3");

	hud = new HUD();
	world = new World();
	enemyInfo = new EnemyInfo();
	if(enemies.find(0) == enemies.end()){
		enemies[0] = new Enemy({ 1000, 430, 0 }, { 128, 128, 1 }, "waddledee", enemyInfo);
	}
}

void KirbyGame::Destroy()
{
	for (int i = 0; i < enemies.size(); i++) {
		SAFE_DELETE(enemies[i]);
	}
	enemies.clear();
	SAFE_DELETE(enemyInfo);
	SAFE_DELETE(kirby);
	SAFE_DELETE(hud);
	SAFE_DELETE(world);
}
void KirbyGame::Update()
{
	if (Keyboard::Get()->Down(VK_F2)) {
		if (Sounds::Get()->IsPaused("Vegetable-Valley.mp3")) {
			Sounds::Get()->Play("Vegetable-Valley.mp3");
		}
		//else if (Sounds::Get()->IsPaused("Vegetable-Valley.mp3")) {
		else {
			Sounds::Get()->Pause("Vegetable-Valley.mp3");
		}
	}
	world->Update();

	for (pair<int, class Enemy*> enemy : enemies) {
		enemy.second->Update();
	}

	kirby->Move();
	kirby->Update();

	//check if kirby is inhaling
	if (kirby->GetState() == inhaling) {
		cout << "kirby is inhaling!" << "\n";
		Vector3 kirbyPos = kirby->GetPosition();
		
		vector<Enemy*> enemySwallowed;
		//check whether kirby is facing right or left
		if (kirby->GetLeft()) {
			//check all enemies on left
			for(int i = 0; i < enemies.size(); i++) {
				Enemy* enemy = enemies[i];
				Vector3 pos = enemy->GetPosition();
				//check enemy in inhaling range
				if (pos.y <= kirbyPos.y + 120.0f && pos.y >= kirbyPos.y - 120.0f &&
					pos.x <= kirbyPos.x && kirbyPos.x - 180.0f < pos.x) {
					cout << "Enemy is in range!" << "\n";
					enemySwallowed.push_back(enemy);
					enemies.erase(i);
					i--;
				}
			}
		}
		else {
			//check all enemies on right
			for (int i = 0; i < enemies.size(); i++) {
				Enemy* enemy = enemies[i];
				Vector3 pos = enemy->GetPosition();
				//check enemy in inhaling range
				if (pos.y <= kirbyPos.y + 120.0f && pos.y >= kirbyPos.y - 120.0f &&
					pos.x >= kirbyPos.x && kirbyPos.x + 180.0f > pos.x) {
					cout << "Enemy is in range!" << "\n";
					enemySwallowed.push_back(enemy);
					enemies.erase(i);
					i--;
				}
			}
		}
		//change kirby state to swallowing
		//all other kirby states are not allowed during this state
		//move enemy through bezier curve
		kirby->SetEnemySwallowed(enemySwallowed);
		kirby->SetState(swallowing);
	}

	kirby->SetHitGround(false);
	kirby->SetHitLeft(false);
	kirby->SetHitRight(false);

	for (size_t j = 0; j < enemies.size(); j++) {
		enemies[j]->SetHitGround(false);
		enemies[j]->SetHitLeft(false);
		enemies[j]->SetHitRight(false);
	}

	//if kirby is in the world
	if (kirby->isKirbyInWorld()) {
		BoundingBox* kirbyBox = kirby->GetRect()->GetBox();
		vector<Rect*> worldRects = world->GetRects();
		for (size_t i = 0; i < worldRects.size(); i++) {
			//if there is collision
			if (BoundingBox::OBB(kirbyBox, worldRects[i]->GetBox())) {
				world->SetColor(i, Values::Blue);
				FixKirbyPosition(worldRects[i]);
			}
			else {
				//transparent gray
				world->SetColor(i, Color(0.5f,0.5f,0.5f,0.7f));
			}
			
			//assume there are enemies in the world
			for (size_t j = 0; j < enemies.size(); j++)
			{
				BoundingBox* enemyBox = enemies[j]->GetRect()->GetBox();
				if (BoundingBox::OBB(enemyBox, worldRects[i]->GetBox())) {
					world->SetColor(i, Values::Red);
					FixEnemyPosition(worldRects[i], j);
				}
				else {
					//transparent gray
					world->SetColor(i, Color(0.5f, 0.5f, 0.5f, 0.7f));
				}
			}
		}
	}

	hud->Update();
	SetCameraBound();
}

void KirbyGame::Render()
{
	world->Render();
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->Render();
	}
	kirby->Render();

	hud->Render();
}

void KirbyGame::PostRender()
{
}

void KirbyGame::GUI()
{
	world->GUI();
}

void KirbyGame::SetCameraBound()
{
	Camera::Get()->SetKirbyPosition(kirby->GetPosition());

	if (kirby->isKirbyInWorld()) {
		Vector3 worldPos = world->getWorldPosition();//TL = TopLeft
		Vector3 worldSize = world->getWorldSize();
		Vector3 worldTL = Vector3(worldPos.x - worldSize.x / 2, worldPos.y + worldSize.y / 2, 0);
		Vector3 worldBR = Vector3(worldPos.x + worldSize.x / 2, worldPos.y - worldSize.y / 2, 0);
		//BR = BottomRight
		
		Vector3 cameraTL = Vector3(worldTL.x, worldTL.y - WinMaxHeight, 0);
		Vector3 cameraBR = Vector3(worldBR.x - WinMaxWidth, worldBR.y, 0);

		Camera::Get()->SetBound(cameraTL, cameraBR);
	}
	else {
		//get level pos
	}
}

void IntersectRect(Rect* kirbyRect, Rect* worldRect, pair<Vector3, Vector3>& intersection) {
	Vector3 kirbyLT = kirbyRect->GetLT();
	Vector3 worldLT = worldRect->GetLT();
	Vector3 kirbyRB = kirbyRect->GetRB();
	Vector3 worldRB = worldRect->GetRB();


	float left = max(kirbyLT.x, worldLT.x);
	float top = min(kirbyLT.y, worldLT.y);
	float right = min(kirbyRB.x, worldRB.x);
	float bottom = max(kirbyRB.y, worldRB.y);
	if (left >= right || top <= bottom) {
		intersection = make_pair(Values::ZeroVec3, Values::ZeroVec3);
		return;
	}
	intersection = make_pair(Vector3(left, top, 0), Vector3(right, bottom, 0));
	return;
}
void KirbyGame::FixKirbyPosition(class Rect* worldRect)
{
	pair<Vector3, Vector3> intersection;
	IntersectRect(kirby->GetRect(), worldRect, intersection);

	Vector3 kirbyPos = kirby->GetRect()->GetPosition();
	Vector3 kirbySize = kirby->GetRect()->GetSize();

	Vector3 kirbyLT = kirby->GetRect()->GetLT();
	Vector3 kirbyRB = kirby->GetRect()->GetRB();

	Vector3 worldRectPos = worldRect->GetPosition();
	Vector3 worldLT = worldRect->GetLT();
	Vector3 worldRB = worldRect->GetRB();

	Vector3 tmp = kirbyPos - worldRectPos;
	if (intersection.first == Values::ZeroVec3 &&
		intersection.second == Values::ZeroVec3) {
		return;
	}
	// Determine the collision direction based on the intersection size
	if (intersection.first.x - intersection.second.x > 
		intersection.second.y - intersection.first.y) {
		if (kirbyLT.x < worldLT.x) {
			// Right collision
			kirbyPos.x = worldLT.x - kirbySize.x / 2 + 1;
			kirby->SetPosition(kirbyPos);
			kirby->SetHitRight(true);
		}
		else {
			// Left collision
			kirbyPos.x = worldRB.x + kirbySize.x / 2 - 1;
			kirby->SetPosition(kirbyPos);
			kirby->SetHitLeft(true);
		}
	}
	else
	{
		if (kirbyLT.y > worldLT.y) {
			// Down collision
			kirbyPos.y = worldLT.y + kirbySize.y / 2 - 1;
			kirby->SetPosition(kirbyPos);
			kirby->SetHitGround(true);
		}
		else {
			// Up collision
			kirbyPos.y = worldRB.y - kirbySize.y / 2;
			kirby->SetPosition(kirbyPos);
		}
	}
}

void KirbyGame::FixEnemyPosition(Rect* worldRect, int idx)
{
	pair<Vector3, Vector3> intersection;
	IntersectRect(enemies[idx]->GetRect(), worldRect, intersection);

	Vector3 enemyPos = enemies[idx]->GetPosition();
	Vector3 enemySize = enemies[idx]->GetRect()->GetSize();

	Vector3 enemyLT = enemies[idx]->GetLT();
	Vector3 enemyRB = enemies[idx]->GetRB();

	Vector3 worldRectPos = worldRect->GetPosition();
	Vector3 worldLT = worldRect->GetLT();
	Vector3 worldRB = worldRect->GetRB();

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
