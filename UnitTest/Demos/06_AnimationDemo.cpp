#include "stdafx.h"
#include "06_AnimationDemo.h"

#include "Geomatries/AnimationRect.h"

void AnimationDemo::Init()
{
	p1 = new AnimationRect({ 720, 360, 0 }, { 200, 200, 0 });
	Sounds::Get()->AddSound("Vegetable-Valley.mp3", 
		SoundPath + L"Vegetable-Valley.mp3", true);
	Sounds::Get()->Play("Vegetable-Valley.mp3");
}

void AnimationDemo::Destroy()
{
	SAFE_DELETE(p1);
}

void AnimationDemo::Update()
{
	p1->Move();
	p1->Update();
	if (Keyboard::Get()->Down(VK_F2)) {
		if (Sounds::Get()->IsPaused("Vegetable-Valley.mp3")) {
			Sounds::Get()->Play("Vegetable-Valley.mp3");
		}
		//else if (Sounds::Get()->IsPaused("Vegetable-Valley.mp3")) {
		else{
			Sounds::Get()->Pause("Vegetable-Valley.mp3");
		}
	}
}

void AnimationDemo::Render()
{
	p1->Render();
}

void AnimationDemo::PostRender()
{
}

void AnimationDemo::GUI()
{
}
