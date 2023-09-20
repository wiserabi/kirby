#include "stdafx.h"
#include "KirbyGame.h"

#include "Geomatries/AnimationRect.h"
#include "KirbyCharacter.h"
#include "World.h"
#include "UI/HUD.h"

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

	hud->Update();
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
}
