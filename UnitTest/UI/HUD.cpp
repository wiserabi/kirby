#include "stdafx.h"
#include "HUD.h"

#include "UI/ProgressBar.h"

HUD::HUD()
{
	HPBar = new ProgressBar({ -322, -298, 0 }, { 720, 160, 0 }, 0.0f,
		TexturePath + L"HUD/Normal.png", UI::LEFT_TO_RIGHT);
}

HUD::~HUD()
{
	SAFE_DELETE(HPBar);
}

void HUD::Update()
{
	if (Keyboard::Get()->Down(VK_F3))
	{
		percent -= 0.1f;
		HPBar->UpdateProgressBar(percent);
	}

	HPBar->Update();

}

void HUD::Render()
{
	HPBar->Render();
}
