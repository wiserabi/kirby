#include "stdafx.h"
#include "Kirby.h"

#include "Geomatries/AnimationRect.h"
#include "KirbyCharacter.h"

#include "UI/HUD.h"

void KirbyGame::Init()
{
	kirby = new KirbyCharacter();
	Sounds::Get()->AddSound("Vegetable-Valley.mp3",
		SoundPath + L"Vegetable-Valley.mp3", true);
	Sounds::Get()->Play("Vegetable-Valley.mp3");

	hud = new HUD();
}

void KirbyGame::Destroy()
{
	SAFE_DELETE(hud);
	SAFE_DELETE(kirby);
}

void KirbyGame::Update()
{
	kirby->Move();
	kirby->Update();
	if (Keyboard::Get()->Down(VK_F2)) {
		if (Sounds::Get()->IsPaused("Vegetable-Valley.mp3")) {
			Sounds::Get()->Play("Vegetable-Valley.mp3");
		}
		//else if (Sounds::Get()->IsPaused("Vegetable-Valley.mp3")) {
		else {
			Sounds::Get()->Pause("Vegetable-Valley.mp3");
		}
	}
	hud->Update();
}

void KirbyGame::Render()
{
	kirby->Render();
	hud->Render();
}

void KirbyGame::PostRender()
{
}

void KirbyGame::GUI()
{
}
