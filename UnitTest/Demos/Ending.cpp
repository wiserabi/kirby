#include "stdafx.h"
#include "Ending.h"
#include "Geomatries/TextureRect.h"

void Ending::Init()
{
	for (size_t i = 0; i < frames; i++)
	{
		if(i > 22){
			path = TexturePath + L"ending/00" + String::ToWString(to_string(i + 77)) + L".png";
		}
		else {
			path = TexturePath + L"ending/000" + String::ToWString(to_string(i + 77)) + L".png";
		}
		endingAnimation.push_back(new TextureRect(position, size, 0.0f, path));
	}
}

void Ending::Destroy()
{
	for (size_t i = 0; i < endingAnimation.size(); i++)
	{
		SAFE_DELETE(endingAnimation[i]);
	}
	endingAnimation.clear();
}

void Ending::Update()
{
	speed++;
	if (curFrame > 0) {
		endingAnimation[curFrame - 1]->Update();
	}
	endingAnimation[curFrame]->Update();
	if (curFrame < frames - 1) {
		if (speed > 2) {
			speed = 0;
			curFrame++;
		}
	}
	else {
		curFrame = 0;
	}
}

void Ending::Render()
{
	if (curFrame > 0) {
		endingAnimation[curFrame - 1]->Render();
	}
	endingAnimation[curFrame]->Render();
}

void Ending::PostRender()
{
}

void Ending::GUI()
{
}
