#include "stdafx.h"
#include "HUD.h"

#include "UI/ProgressBar.h"

HUD::HUD()
{
	float width = -WinMaxWidth / 2 + 30;
	float height = WinMaxHeight / 2 - 30;
	HPBar = new ProgressBar({ -372, -256, 0 }, { 744, 192, 0 }, 0.0f, 
		TexturePath + L"HUD/mapUi.png", UI::LEFT_TO_RIGHT);
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
