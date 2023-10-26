#include "stdafx.h"
#include "HUD.h"

#include "UI/ProgressBar.h"
#include "Utilities/Animator.h"
#include "Geomatries/TextureRect.h"

HUD::HUD()
{
	frameUI = new ProgressBar({ -322.0f, -274.0f, 0.0f }, { 740.0f, 160.0f, 0.0f }, 0.0f,
		HudPath + L"Normal.png", UI::LEFT_TO_RIGHT);
	LoadStateImage();
	LoadNumberImage();
	LoadHealthImage();
	LoadLifeImage();

}

HUD::~HUD()
{
	SAFE_DELETE(frameUI);
	vector<ProgressBar*>().swap(stateImg);
	vector<ProgressBar*>().swap(health);
	vector<ProgressBar*>().swap(number);
	vector<ProgressBar*>().swap(life);
}

void HUD::Update()
{
	frameUI->Update();
	if (prevState != 27 && state == 27 && hpLeft > 0) {
		hpLeft--;
		if (hpLeft == 0 && lifeLeft > 0) {
			hpLeft = 6;
			lifeLeft--;
		}
	}
	if (state == 27) {//hit by enemy
		stateImg[0]->Update();
	}
	else if (ability == 0) {//spark
		stateImg[1]->Update();
	}
	else if (ability == 1) {//beam
		stateImg[2]->Update();
	}
	else if (ability == 8) {
		stateImg[4]->Update();
	}

	if (Time::Get()->Running() - healthAnimStart > 0.1f) {
		if (hpAnimIdx) {
			hpAnimIdx = false;
		}
		else {
			hpAnimIdx = true;
		}
		healthAnimStart = Time::Get()->Running();
	}
	if (hpAnimIdx) {
		for (int i = 0; i < hpLeft; i++) {
			health[i]->Update();
		}
	}
	else {
		for (int i = 6; i < 6 + hpLeft; i++) {
			health[i]->Update();
		}
	}

	for (int i = 17; i > 11 + hpLeft; i--) {
		health[i]->Update();
	}
	if (Time::Get()->Running() - lifeAnimStart > 0.2f) {
		lifeAnimIdx = (lifeAnimIdx + 1) % 4;
		lifeAnimStart = Time::Get()->Running();
	}

	life[lifeAnimIdx]->Update();
	number[0]->Update();
	number[1]->Update();
	number[2]->Update();
	number[3]->Update();
	number[4]->Update();
	number[5]->Update();
	number[6]->Update();

	number[7]->Update();
	number[8 + 9 * lifeLeft]->Update();

}

void HUD::Render()
{
	frameUI->Render();
	if (state == 27) {//hit by enemy
		stateImg[0]->Render();
	}
	else if (ability == 0) {//spark
		stateImg[1]->Render();
	}
	else if (ability == 1) {//beam
		stateImg[2]->Render();
	}
	else if (ability == 8) {
		stateImg[4]->Render();
	}
	
	if (hpAnimIdx) {
		for (int i = 0; i < hpLeft; i++) {
			health[i]->Render();
		}
	}
	else {
		for (int i = 6; i < 6 + hpLeft; i++) {
			health[i]->Render();
		}
	}
	for (int i = 17; i > 11 + hpLeft; i--) {
		health[i]->Render();
	}
	life[lifeAnimIdx]->Render();
	number[0]->Render();
	number[1]->Render();
	number[2]->Render();
	number[3]->Render();
	number[4]->Render();
	number[4]->Render();
	number[5]->Render();
	number[6]->Render();
	
	number[7]->Render();
	
	number[8 + 9 * lifeLeft]->Render();//life

}

void HUD::LoadStateImage()
{

	for (size_t i = 0; i < 6; i++)
	{
		stateImg.push_back(new ProgressBar({ 108.0f,-274.0f, 0.0f }, 
			{ 95.0f, 114.0f, 0.0f }, 0.0f,
			HudPath + statePng[i], UI::LEFT_TO_RIGHT));
	}
}

void HUD::LoadLifeImage()
{
	for (size_t i = 0; i < 3; i++) {
		life.push_back(new ProgressBar({ 236.0f,-269.0f, 0.0f },
			{ 40.0f, 36.0f, 0.0f }, 0.0f,
			HudPath + lifePng[i], UI::LEFT_TO_RIGHT));
	}
	life.push_back(new ProgressBar({ 236.0f,-269.0f, 0.0f },
		{ 40.0f, 36.0f, 0.0f }, 0.0f,
		HudPath + lifePng[1], UI::LEFT_TO_RIGHT));
}

void HUD::LoadHealthImage()
{
	for (size_t i = 0; i < 3; i++) {
		health.push_back(new ProgressBar({ -108.0f,-251.0f, 0.0f },
			{ 28.0f, 40.0f, 0.0f }, 0.0f,
			HudPath + healthPng[i], UI::LEFT_TO_RIGHT));
		health.push_back(new ProgressBar({ -80.0f,-251.0f, 0.0f },
			{ 28.0f, 40.0f, 0.0f }, 0.0f,
			HudPath + healthPng[i], UI::LEFT_TO_RIGHT));
		health.push_back(new ProgressBar({ -52.0f,-251.0f, 0.0f },
			{ 28.0f, 40.0f, 0.0f }, 0.0f,
			HudPath + healthPng[i], UI::LEFT_TO_RIGHT));
		health.push_back(new ProgressBar({ -24.0f,-251.0f, 0.0f },
			{ 28.0f, 40.0f, 0.0f }, 0.0f,
			HudPath + healthPng[i], UI::LEFT_TO_RIGHT));
		health.push_back(new ProgressBar({ 4.0f,-251.0f, 0.0f },
			{ 28.0f, 40.0f, 0.0f }, 0.0f,
			HudPath + healthPng[i], UI::LEFT_TO_RIGHT));
		health.push_back(new ProgressBar({ 32.0f,-251.0f, 0.0f },
			{ 28.0f, 40.0f, 0.0f }, 0.0f,
			HudPath + healthPng[i], UI::LEFT_TO_RIGHT));
	}
}

void HUD::LoadNumberImage()
{
	for (size_t i = 0; i < 10; i++) {
		number.push_back(new ProgressBar({ -107.0f,-296.0f, 0.0f },
			{ 24.0f, 24.0f, 0.0f }, 0.0f,
			HudPath + numberPng[i], UI::LEFT_TO_RIGHT));
		number.push_back(new ProgressBar({ -83.0f,-296.0f, 0.0f },
			{ 24.0f, 24.0f, 0.0f }, 0.0f,
			HudPath + numberPng[i], UI::LEFT_TO_RIGHT));
		number.push_back(new ProgressBar({ -59.0f,-296.0f, 0.0f },
			{ 24.0f, 24.0f, 0.0f }, 0.0f,
			HudPath + numberPng[i], UI::LEFT_TO_RIGHT));
		number.push_back(new ProgressBar({ -35.0f,-296.0f, 0.0f },
			{ 24.0f, 24.0f, 0.0f }, 0.0f,
			HudPath + numberPng[i], UI::LEFT_TO_RIGHT));
		number.push_back(new ProgressBar({ -11.0f,-296.0f, 0.0f },
			{ 24.0f, 24.0f, 0.0f }, 0.0f,
			HudPath + numberPng[i], UI::LEFT_TO_RIGHT));
		number.push_back(new ProgressBar({ 13.0f,-296.0f, 0.0f },
			{ 24.0f, 24.0f, 0.0f }, 0.0f,
			HudPath + numberPng[i], UI::LEFT_TO_RIGHT));
		number.push_back(new ProgressBar({ 37.0f,-296.0f, 0.0f },
			{ 24.0f, 24.0f, 0.0f }, 0.0f,
			HudPath + numberPng[i], UI::LEFT_TO_RIGHT));

		number.push_back(new ProgressBar({ 299.0f,-273.0f, 0.0f },
			{ 24.0f, 24.0f, 0.0f }, 0.0f,
			HudPath + numberPng[i], UI::LEFT_TO_RIGHT));
		number.push_back(new ProgressBar({ 323.0f,-273.0f, 0.0f },
			{ 24.0f, 24.0f, 0.0f }, 0.0f,
			HudPath + numberPng[i], UI::LEFT_TO_RIGHT));

	}
}

void HUD::SetCurrentAbility(int ability)
{
	this->ability = ability;
}

void HUD::SetState(int state)
{
	this->state = state;
}

void HUD::SetPrevState(int prevState)
{
	this->prevState = prevState;
}
