#include "stdafx.h"
#include "BezierCurves.h"
#include "KirbyEffect.h"
#include "Geomatries/AnimationRect.h"
#include "Geomatries/Rect.h"

#include "Utilities/Animator.h"
#include "Enemy.h"

#define THROWSTARSPEED 200

KirbyEffect::KirbyEffect()
{
	LoadTextureList();
}

KirbyEffect::~KirbyEffect()
{

}

void KirbyEffect::SetKirbyPos(Vector3 kirbyPos, bool left)
{
	this->kirbyPos = kirbyPos;
	this->left = left;
}

void KirbyEffect::LoadTextureList()
{
	for (size_t i = 0; i < 16; i++)
	{
		{
			Texture2D* srcTex = new Texture2D(TexturePath + L"kirbyEffect/" + pngList[i]);
			AnimationClip* tmpClip = new AnimationClip(pngList[i], srcTex, pngSplit[i],
				Vector2(0, srcTex->GetHeight() * 0.0f),
				Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 1.0f));
			//add clip for each animator
			animatorList.push_back(new Animator());
			animatorList[i]->AddAnimClip(tmpClip);
			animatorList[i]->SetCurrentAnimClip(pngList[i]);
			SAFE_DELETE(srcTex);
		}
	}

}

/*
Eat Effect Animation

For each waterdrops on bezeir curve
kirby's location is p1
The waterdrop move from p3 to p1 
When waterdrop reaches p1 it gets removed
Everytime waterdrop starts at p3 running time is saved
Using when p3 started last time next p3 start time is
calculated using random float from 0.1 ~ 0.5 sec
*/
void KirbyEffect::SetKirbyEat()
{

	currentEffect = Effect::eat;
	//create 50 animationRect
	for (int i = 0; i < 50; i++) {
		animations.push_back(new AnimationRect(Values::ZeroVec3, Vector3(96.0f, 96.0f, 0.0f), false));
		animations[i]->SetAnimator(animatorList[currentEffect]);
	}

	curves = new BezierCurves();
	
	//add control points for bezier curve
	if (controlPoints.size()) {
		controlPoints.clear();
	}
	Vector2 tmpP1 = Vector2(kirbyPos.x, kirbyPos.y);
	float xDistance[2] = { 100.0f, 150.0f };
	float yDistance[2] = { 25.0f, 70.0f };

	if (left) {
		xDistance[0] = -xDistance[0];
		xDistance[1] = -xDistance[1];
	}
	//set bezeir curves for eat effect;
	for (int i = 0; i < 4; i++) {
		Vector2 tmpP2 = Vector2(tmpP1.x + xDistance[0], tmpP1.y + (2 - i) * yDistance[0]);
		Vector2 tmpP3 = Vector2(tmpP1.x + xDistance[1], tmpP1.y + (2 - i) * yDistance[1]);
		curves->Add(tmpP1, tmpP2, tmpP3);
	}
}

void KirbyEffect::SetKirbySwallow(vector<pair<class Enemy*, int>>& enemySwallowed)
{
	currentEffect = Effect::swallowing;
	curves = new BezierCurves();
	//set kirby pos as p1
	Vector2 tmpP1 = Vector2(kirbyPos.x, kirbyPos.y);
	//get position of enemy and create curves
	for (size_t i = 0; i < enemySwallowed.size(); i++)
	{
		Vector3 enemyPos = enemySwallowed[i].first->GetPosition();
		Vector2 tmpP3 = { enemyPos.x, enemyPos.y };
		Vector2 tmpP2 = { (enemyPos.x + kirbyPos.x) / 2.0f, kirbyPos.y };
		curves->Add(tmpP1, tmpP2, tmpP3);
		curves->CreateWaterdrop(i);
	}
}

void KirbyEffect::SetKirbyBlowStar()
{
	currentEffect = Effect::bigstars;
	animations.push_back(new AnimationRect(Values::ZeroVec3, Vector3(96.0f, 96.0f, 0.0f), false));
	animations[0]->SetAnimator(animatorList[currentEffect]);
	//get current position to start effect and direction(left or right)
	effectStartPos = Vector3(kirbyPos.x, kirbyPos.y, 0.0f);
	effectLeft = left;

	rectEffect0 = new Rect(effectStartPos, Vector3(42.0f, 42.0f, 0.0f), 0.0f);
	rectEffect0->SetColor(Color(0.5f, 0.5f, 0.5f, 0.7f));
}

void KirbyEffect::SetKirbyStarExplodeOnEnemy(Vector3 pos)
{
	currentEffect = Effect::explode;
	animations.push_back(new AnimationRect(pos, Vector3(96.0f, 96.0f, 0.0f), false));
	animations[0]->SetAnimator(animatorList[currentEffect]);
	//get current position to start effect and direction(left or right)
	effectStartPos = Vector3(pos.x, pos.y, 0.0f);
}

void KirbyEffect::SetEnemyDeathEffect(Vector3 pos)
{
	currentEffect = Effect::bigstars;
	for (int i = 0; i < 4; i++) {
		animations.push_back(new AnimationRect(pos, Vector3(52.0f, 52.0f, 0.0f), false));
		animations[i]->SetAnimator(animatorList[currentEffect]);
		effectStartPositions.push_back(pos);
	}
}

//when kirby is hit by enemy
void KirbyEffect::SetHitEffect()
{
	currentEffect = Effect::bigstars;
	animations.push_back(new AnimationRect(kirbyPos, Vector3(52.0f, 52.0f, 0.0f), false));
	animations[0]->SetAnimator(animatorList[currentEffect]);
	//get current position to start effect and direction(left or right)
	effectStartPos = kirbyPos;
	int randDir = rand() % 8;//set random direction
	if (randDir == 0) {
		randVec = Values::LeftVec;
	}
	else if (randDir == 1) {
		randVec = Values::RightVec;
	}
	else if (randDir == 2) {
		randVec = Values::UpVec;
	}
	else if (randDir == 3) {
		randVec = Values::DownVec;
	}
	else if (randDir == 4) {
		randVec = Values::RightUpVec;
	}
	else if (randDir == 5) {
		randVec = Values::RightDownVec;
	}
	else if (randDir == 6) {
		randVec = Values::LeftDownVec;
	}
	else if (randDir == 7) {
		randVec = Values::LeftUpVec;
	}

}

void KirbyEffect::SetKirbyBlowAir()
{
	effectStartPos = kirbyPos;
	currentEffect = Effect::blow;
	animations.push_back(new AnimationRect(effectStartPos, Vector3(96.0f, 96.0f, 0.0f), false));
	animations[0]->SetAnimator(animatorList[currentEffect]);
	//get current position to start effect and direction(left or right)
	rectEffect0 = new Rect(effectStartPos, Vector3(52.0f, 52.0f, 0.0f), 0.0f);
}

void KirbyEffect::SetSparkEffect(Vector3 pos)
{
	currentEffect = Effect::spark;

	effectStartPos = pos;
	animations.push_back(new AnimationRect(effectStartPos, Vector3(270.0f, 270.0f, 0.0f), false));
	animations[0]->SetAnimator(animatorList[currentEffect]);
	animatorList[currentEffect]->SetPlayRate(pngList[15],1.0f / 40.0f);

	rectEffect0 = new Rect(effectStartPos, Vector3(160.0f, 160.0f, 0.0f), 0.0f);
}

void KirbyEffect::SetBeamEffect(Vector3 pos, bool leftSide)
{
	currentEffect = Effect::beam;

	if (leftSide) {
		effectStartPos = pos + Values::LeftUpVec * 50 + Values::LeftVec * 20 ;//effect starting point
		animations.push_back(new AnimationRect(effectStartPos, Vector3(128.0f, 128.0f, 0.0f), false));
		animations[0]->SetAnimator(animatorList[currentEffect]);
		animations[0]->SetLeft(leftSide);
	}
	else {
		effectStartPos = pos + Values::RightUpVec * 50 + Values::RightVec * 20;//effect starting point
		animations.push_back(new AnimationRect(effectStartPos, Vector3(128.0f, 128.0f, 0.0f), false));
		animations[0]->SetAnimator(animatorList[currentEffect]);
		animations[0]->SetLeft(leftSide);
	}
	animatorList[currentEffect]->SetPlayRate(pngList[0], 1.0f / 30.0f);

	rectEffect0 = new Rect(effectStartPos, Vector3(128.0f, 128.0f, 0.0f), 0.0f);
}

void KirbyEffect::UpdateEatEffect()
{
	if (time + duration < Time::Get()->Running()) {
		count = 0;
		for (auto& it : animations) {
			SAFE_DELETE(it);
		}
		//clear the previous effect animations
		vector<AnimationRect*>().swap(animations);
		curves->Clear();
		SAFE_DELETE(curves);
		setTimer = false;
		return;
	}

	curves->Update(true);
	vector<Line> lines_ = curves->GetLines();
	
	count = 0;
	//set position for animationRects and update
	for (int i = 0; i < lines_.size(); i++) {
		Line& line = lines_[i];
		for (int j = 0; j < line.water_drops.size(); j++) {
			if (count >= 50) {
				//max 50 eat particles 
				return;
			}
			animations[count]->SetPosition(Vector3(line.water_drops[j].x, line.water_drops[j].y, 0.0f));
			animations[count]->Update(animatorList[currentEffect]);
			count++;
		}
	}
}

bool KirbyEffect::UpdateSwallowEffect(vector<pair<class Enemy*, int>>& enemySwallow)
{
	if (time + duration < Time::Get()->Running()) {
		curves->Clear();
		delete curves;
		curves = nullptr;
		setTimer = false;
		return true;
	}
	bool result = true;
	//first update the curves
	curves->Update(false);
	vector<Line> lines_ = curves->GetLines();

	//change position of enemies being swallowed according to the lines position
	for (int i = 0; i < enemySwallow.size(); i++) {
		if (i >= 50) {
			break;
		}
		//number of lines == number of enemy being swallowed
		Line& line = lines_[i];
		//if enemy pos == kirby pos, water drop will be removed
		if (line.water_drops.size() == 0) {
			continue;
		}
		result = result & false;
		for (int j = 0; j < line.water_drops.size(); j++) {
			enemySwallow[i].first->ChangeAnimation(L"", 0.0f, Values::ZeroVec3, enemySwallow[i].second, true);
			enemySwallow[i].first->SetPosition(Vector3(line.water_drops[j].x, line.water_drops[j].y, 0.0f));
			enemySwallow[i].first->Update();
			count++;
		}
	}
	return result;
}
//blow star effect
bool KirbyEffect::UpdateBlowEffect(float deltaTime)
{
	if (time + duration < Time::Get()->Running()) {
		SAFE_DELETE(rectEffect0);
		animations.clear();
		setTimer = false;
		return true;
	}
	//move effect left
	if (effectLeft) {
		effectStartPos += Values::LeftVec * 5;
	}
	else {
		effectStartPos += Values::RightVec * 5;
	}

	rectEffect0->SetPosition(effectStartPos);
	rectEffect0->Update();

	animations[0]->SetPosition(effectStartPos);
	animations[0]->Update(animatorList[currentEffect]);

	return false;
}

void KirbyEffect::UpdateExplodeOnEnemy()
{
	if (time + duration < Time::Get()->Running()) {
		StopEffect();
		return;
	}
	animations[0]->ChangeAnimation(L"", 0.0f, Values::ZeroVec3, 1, true);
	animations[0]->Update(animatorList[currentEffect]);
}

void KirbyEffect::UpdateDeathEffect(float delta)
{
	if (time + duration < Time::Get()->Running()) {
		animations.clear();
		setTimer = false;
		return;
	}
	//up right
	effectStartPositions[0] += Values::RightUpVec * 100 * delta;
	//up left
	effectStartPositions[1] += Values::LeftUpVec * 100 * delta;
	//down right
	effectStartPositions[2] += Values::RightDownVec * 100 * delta;
	//down left
	effectStartPositions[3] += Values::LeftDownVec * 100 * delta;

	for (int i = 0; i < animations.size(); i++) {
		animations[i]->SetPosition(effectStartPositions[i]);
		animations[i]->ChangeAnimation(L"", 0.0f, Values::ZeroVec3, 0, true);
		animations[i]->Update(animatorList[currentEffect]);
	}
}

void KirbyEffect::UpdateHitEffect(float delta)
{
	if (time + duration < Time::Get()->Running()) {
		animations.clear();
		setTimer = false;
		return;
	}
	effectStartPos += randVec * 150 * delta;
	animations[0]->SetPosition(effectStartPos);
	animations[0]->ChangeAnimation(L"", 0.0f, Values::ZeroVec3, 1, true);
	animations[0]->Update(animatorList[currentEffect]);
}

void KirbyEffect::UpdateBlowAir(float delta)
{
	if (time + duration < Time::Get()->Running()) {
		StopEffect();
		return;
	}
	if (left) {
		effectStartPos += Values::LeftVec * 300 * delta;
	}
	else {
		effectStartPos += Values::RightVec * 300 * delta;
	}

	rectEffect0->SetPosition(effectStartPos);
	rectEffect0->Update();

	animations[0]->SetLeft(left);
	animations[0]->SetPosition(effectStartPos);
	animations[0]->ChangeAnimation(L"", 0.0f, Values::ZeroVec3, 0, false);
	animations[0]->Update(animatorList[currentEffect]);
}

void KirbyEffect::UpdateSparkEffect(float delta)
{
	if (time + duration < Time::Get()->Running()) {
		animations.clear();
		SAFE_DELETE(rectEffect0);
		setTimer = false;
		return;
	}
	animations[0]->SetPosition(effectStartPos);
	animations[0]->Update(animatorList[currentEffect]);

	rectEffect0->SetPosition(effectStartPos);
	rectEffect0->Update();
}

void KirbyEffect::UpdateBeamEffect(float delta, float leftSide)
{
	if (time + duration < Time::Get()->Running()) {
		animations.clear();
		SAFE_DELETE(rectEffect0);
		setTimer = false;
		return;
	}
	animations[0]->SetPosition(effectStartPos);
	animations[0]->SetLeft(leftSide);
	animations[0]->Update(animatorList[currentEffect]);

	rectEffect0->SetPosition(effectStartPos);
	rectEffect0->Update();
}

void KirbyEffect::StartTimer(float duration)
{
	setTimer = true;
	this->duration = duration;
	time = Time::Get()->Running();
}


void KirbyEffect::UpdateEffect(float deltaTime)
{
	if (currentEffect == Effect::eat && setTimer) {
		UpdateEatEffect();
		return;
	}
	if (currentEffect == Effect::bigstars && setTimer) {
		UpdateBlowEffect(deltaTime);
		return;
	}
	if (currentEffect == Effect::explode && setTimer) {
		UpdateExplodeOnEnemy();
		return;
	}
}

void KirbyEffect::RenderSwallowEffect(vector<pair<class Enemy*, int>>& enemySwallow) {
	for (int i = 0; i < enemySwallow.size(); i++) {
		enemySwallow[i].first->Render();
	}
}
void KirbyEffect::RenderDeathEffect()
{
	for (int i = 0; i < animations.size(); i++) {
		animations[i]->Render();
	}
}

void KirbyEffect::RenderHitEffect()
{
	if (animations.size()) {
		animations[0]->Render();
	}
}

void KirbyEffect::RenderBlowAir()
{
	if (animations.size() && rectEffect0) {
		rectEffect0->Render();
		animations[0]->Render();
	}
}

void KirbyEffect::RenderSparkEffect()
{
	if (animations.size()) {
		rectEffect0->Render();
		animations[0]->Render();
	}
}

void KirbyEffect::RenderBeamEffect()
{
	if (animations.size()) {
		rectEffect0->Render();
		animations[0]->Render();
	}
}

void KirbyEffect::RenderEffect()
{
	if (currentEffect == Effect::eat && animations.size()) {
		for (int i = 0; i < count; i++) {
			animations[i]->Render(animatorList[currentEffect]);
		}
	}
	else if (currentEffect == Effect::bigstars && animations.size() && rectEffect0) {
		//render if there is animations
		rectEffect0->Render();
		animations[0]->Render(animatorList[currentEffect]);
	}
	else if (currentEffect == Effect::explode && animations.size()) {
		//render if there is animations
		animations[0]->Render(animatorList[currentEffect]);
	}
	else if (currentEffect == Effect::blow) {
		RenderBlowAir();
	}
}

void KirbyEffect::StopEffect()
{
	if (currentEffect == Effect::bigstars) {
		vector<AnimationRect*>().swap(animations);

		SAFE_DELETE(rectEffect0);
		setTimer = false;
		return;
	}
	if (currentEffect == Effect::explode) {
		vector<AnimationRect*>().swap(animations);
		setTimer = false;
		return;
	}
	if (currentEffect == Effect::blow) {
		vector<AnimationRect*>().swap(animations);
		SAFE_DELETE(rectEffect0);
		setTimer = false;
		return;
	}

}