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
	path = TexturePath + L"ending/endingCredit.png";
	endingCredit = new TextureRect(position, {481.0f,114.0f,0.0f}, 0.0f, path);
}

void Ending::Destroy()
{
	for (size_t i = 0; i < endingAnimation.size(); i++)
	{
		SAFE_DELETE(endingAnimation[i]);
	}
	endingAnimation.clear();
	SAFE_DELETE(endingCredit);
}

void Ending::Update()
{
	if (end) {
		endingAnimation[0]->Update();
		endingCredit->Update();
		return;
	}
	speed++;

	if (curFrame > 0) {
		endingAnimation[curFrame - 1]->Update();
	}
	else if (curFrame == 0 && speed < 100) {
		endingAnimation[0]->Update();
		return;
	}
	else if (curFrame == 0 && speed >= 100) {
		speed = 0;
		curFrame = 1;
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
		end = true;
	}
}

void Ending::Render()
{
	if (curFrame > 0) {
		endingAnimation[curFrame - 1]->Render();
	}
	endingAnimation[curFrame]->Render();
	endingCredit->Render();
}

void Ending::PostRender()
{
}

void Ending::GUI()
{
}
