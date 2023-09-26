#include "stdafx.h"
#include "KirbyGame.h"

#include "Geomatries/AnimationRect.h"
#include "KirbyCharacter.h"
#include "World.h"
#include "UI/HUD.h"
#include "Geomatries/Rect.h"

#define THRESHOLD 2
void KirbyGame::Init()
{
	kirby = new KirbyCharacter({ 720, 360, 1.0 }, { 128, 128, 1 });
	Sounds::Get()->AddSound("Vegetable-Valley.mp3",
		SoundPath + L"Vegetable-Valley.mp3", true);
	Sounds::Get()->Play("Vegetable-Valley.mp3");

	hud = new HUD();
	world = new World();
}

void KirbyGame::Destroy()
{
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

	kirby->Move();
	kirby->Update();

	kirby->SetHitGround(false);
	//if kirby is in the world
	if (kirby->isKirbyInWorld()) {
		BoundingBox* kirbyBox = kirby->GetRect()->GetBox();
		vector<Rect*> worldRects = world->GetRects();
		for (int i = 0; i < worldRects.size(); i++) {
			//check if kirby is on the ground
			if (worldRects[i]->GetLT().y + THRESHOLD > kirby->GetRect()->GetRB().y &&
				worldRects[i]->GetLT().y - THRESHOLD < kirby->GetRect()->GetRB().y &&
				!(worldRects[i]->GetLT().x > kirby->GetRect()->GetRB().x || 
				worldRects[i]->GetRB().x < kirby->GetRect()->GetLT().x)) {
				kirby->SetHitGround(true);
			}
			//if there is collision
			if (BoundingBox::OBB(kirbyBox, worldRects[i]->GetBox())) {
				world->SetColor(i, Values::Blue);
				FixKirbyPosition(worldRects[i]);
			}
			else {
				//transparent gray
				world->SetColor(i, Color(0.5,0.5,0.5,0.7));
			}
		}
	}

	hud->Update();
	SetCameraBound();
}

void KirbyGame::Render()
{
	world->Render();
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
			kirbyPos.x = worldLT.x - kirbySize.x / 2;
			kirby->SetPosition(kirbyPos);
		}
		else {
			// Left collision
			kirbyPos.x = worldRB.x + kirbySize.x / 2;
			kirby->SetPosition(kirbyPos);
		}
	}
	else
	{
		if (kirbyLT.y > worldLT.y) {
			// Down collision
			kirbyPos.y = worldLT.y + kirbySize.y / 2;
			kirby->SetPosition(kirbyPos);
		}
		else {
			// Up collision
			kirbyPos.y = worldRB.y - kirbySize.y / 2;
			kirby->SetPosition(kirbyPos);
		}
	}
}
