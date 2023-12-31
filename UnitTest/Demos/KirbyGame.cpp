#include "stdafx.h"
#include "KirbyGame.h"
#include "Geomatries/Rect.h"
#include "Geomatries/TextureRect.h"

#include "Geomatries/AnimationRect.h"
#include "KirbyCharacter.h"
#include "World.h"
#include "Level.h"
#include "UI/HUD.h"
#include "Geomatries/Rect.h"
#include "Enemy.h"
#include "EnemyInfo.h"
#include "KirbyEffect.h"

void KirbyGame::Init()
{
	AddSounds();

	kirby = new KirbyCharacter({ 720, 360, 1.0 }, { 128, 128, 1 });
	Sounds::Get()->Play(sounds[0], volume);
	slopeRange.push_back({ 10092.0f , 10232.0f });//up 45degree
	slopeRange.push_back({ 10232.0f , 10502.0f });//up 27degree
	slopeRange.push_back({ 10590.0f , 10854.0f });//down 27degree
	slopeRange.push_back({ 10854.0f , 10994.0f });//down 45degree
	slopeRange.push_back({ 11468.0f , 11746.0f });//up 27degree
	slopeRange.push_back({ 12384.0f , 12790.0f });//down 27degree

	hud = new HUD();
	world = new World();

	effects.push_back(new KirbyEffect());//used for kirby inhaling
	effects.push_back(new KirbyEffect());//pulling enemy as effect
	effects.push_back(new KirbyEffect());//kirby blow star effect
	effects.push_back(new KirbyEffect());//explode frame 1(explode when star hits enemy)
	effects.push_back(new KirbyEffect());//kirby hit enemy effect
	effects.push_back(new KirbyEffect());//kirby blow air effect
}

void KirbyGame::Destroy()
{
	for (size_t i = 0; i < effects.size(); i++)
	{
		SAFE_DELETE(effects[i]);
	}
	effects.clear();
	slopeRange.clear();
	sounds.clear();

	SAFE_DELETE(kirby);
	SAFE_DELETE(hud);
	SAFE_DELETE(world);
}

void KirbyGame::Update()
{
	kirbyCurState = kirby->GetState();
	kirbyPrevState = kirby->GetPrevState();

	kirbyLocation = kirby->getKirbyLocation();
	world->SetKirbyLocation(kirbyLocation);
	
	Sound();
	kirbyPrevLocation = kirbyLocation;
	world->SetKirbyPos(kirby->GetPosition());

	world->Update();

	kirby->Move();
	kirby->Update();
	
	//cout << kirbyPrevState << " : " << kirbyCurState << "\n";

	//set blow air effect of kirby
	if (kirbyPrevState == inhaled && kirbyCurState == exhaling) {
		SetKirbyBlowAir();
	}
	//kirby state change inhale => inhaling
	else if (kirbyPrevState == inhale && kirbyCurState == inhaling) {
		SetKirbyEat();
	}
	else if (kirbyPrevState == inhaling && kirbyCurState == stopInhaling) {
		//kirby stopped inhaling moment
		effects[0]->StartTimer(0.1f);
	}
	//when changed to throw star
	else if ((kirbyPrevState == eatidle || kirbyPrevState == eatandwalk) && kirbyCurState == attacking) {
		SetThrowStar();
	}

	kirby->SetHitGround(false);
	kirby->SetHitLeft(false);
	kirby->SetHitRight(false);

	//get levels from world
	vector<Level*> levels = world->GetLevels();
	prevDeathCnt = deathCnt;
	deathCnt = 0;
	for (size_t i = 0; i < levels.size(); i++)
	{
		deathCnt += levels[i]->GetEnemyDeathCount();
	}

	if (Keyboard::Get()->Down(VK_F5)) {
		if (invincibleDuration < 3.0f) {
			invincibleDuration = 100000.0f;
			dmg = 50;
		}
		else {
			invincibleDuration = 2.0f;
			dmg = 5;
		}
	}

	//if kirby is in the world
	if (kirbyLocation == WORLD) {
		startboss = false;//started boss fight
		hud->ChangeToNormalUI();//change to boss ui

		BoundingBox* kirbyBox = kirby->GetRect()->GetBox();
		vector<Rect*> worldRects = world->GetRects();
		for (size_t i = 0; i < worldRects.size(); i++) {
			KirbyCollisionWithWorld(kirbyBox, worldRects[i]);

			effectBox = nullptr;
			effectRect = effects[2]->GetRect();
			if (effectRect) {
				effectBox = effectRect->GetBox();
			}
			//check if big star thrown by kirby hits the wall
			if (effectRect && effectBox && BoundingBox::OBB(effectBox, worldRects[i]->GetBox())) {
				//check if effect is alive
				if (effects[2]->isTimerSet()) {
					effects[2]->StartTimer(0.0f);
				}
			}
		}
		SetCameraBound();
	}
	else {//kirby in levels
		BoundingBox* kirbyBox = kirby->GetRect()->GetBox();
		vector<Rect*> levelRects = levels[kirbyLocation - 1]->GetRects();

		hud->SetEnemyDeathCnt(deathCnt);

		prevTimerSet = curTimerSet;
		curTimerSet = effects[1]->isTimerSet();
		//timer for effects[1] finished? inhaling enemy finish?
		if (prevTimerSet && !curTimerSet) {
			kirby->SetAttackDelay();
			kirby->SetState(eatidle);
			effects[0]->StartTimer(0.1f);
		}
		if (kirbyCurState == inhaling && !effects[1]->isTimerSet()) {
			CheckInhaleEnemy(kirbyLocation - 1, levels);
		}
		//interaction between enemy and kirby
		EnemyAndKirby(kirbyLocation - 1, levels);
		//interaction between boss and kirby
		if (kirbyLocation == 4) {
			BossAndKirby(kirbyLocation - 1, levels);

			bool curStartBoss = levels[kirbyLocation - 1]->GetStartBoss();
			if (!startboss && curStartBoss) {
				startboss = true;//started boss fight
				hud->ChangeToBossUI();//change to boss ui
			}
			//set boss health for hud if boss fight has started
			if (curStartBoss) {
				hud->SetBossHealth(levels[kirbyLocation - 1]->GetBossHealth());
			}
		}

		//iterate level rects for kirby collision with level
		for (size_t i = 0; i < levelRects.size(); i++) {
			KirbyCollisionWithWorld(kirbyBox, levelRects[i]);
			
			BoundingBox* effectBox = nullptr;
			Rect* effectRect = effects[2]->GetRect();
			if (effectRect) {
				effectBox = effectRect->GetBox();
			}
			//check if big star thrown by kirby hits the wall
			if (effectRect && effectBox && BoundingBox::OBB(effectBox, levelRects[i]->GetBox())) {
				//check if effect is alive
				if (effects[2]->isTimerSet()) {
					effects[2]->StartTimer(0.0f);
				}
			}
		}
		Vector3 kirbyPos = kirby->GetPosition();
		SetCameraBound();
	}
	hud->SetCurrentAbility(kirby->GetCurrentAbility());
	hud->SetState(kirby->GetState());
	hud->SetPrevState(kirby->GetPrevState());
	hud->SetNothing(kirby->GetNothing());
	
	hud->Update();
	//check if there is a timer set for animation
	UpdateEffect();
}

void KirbyGame::Render()
{
	world->Render();

	for (size_t i = 0; i < effects.size(); i++)
	{
		if (effects[i]->isTimerSet()) {
			effects[i]->RenderEffect();
		}
	}
	effects[1]->RenderSwallowEffect();
	effects[4]->RenderHitEffect();

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

	if (kirby->getKirbyLocation() == WORLD) {
		Vector3 worldLT = world->GetLT();
		Vector3 worldBR = world->GetRB();
		//BR = BottomRight
		
		Vector3 cameraTL = Vector3(worldLT.x, worldLT.y - WinMaxHeight, 0);
		Vector3 cameraRB = Vector3(worldBR.x - WinMaxWidth, worldBR.y, 0);

		Camera::Get()->SetBound(cameraTL, cameraRB);
	}
	else if(kirby->getKirbyLocation() == LEVEL1){
		vector<class Level*> levels = world->GetLevels();
		
		Vector3 levelLT = levels[0]->GetLT();
		Vector3 levelRB = levels[0]->GetRB();
		//BR = BottomRight

		Vector3 cameraTL = Vector3(levelLT.x, levelLT.y - WinMaxHeight, 0);
		Vector3 cameraRB = Vector3(levelRB.x - WinMaxWidth, levelRB.y, 0);

		Camera::Get()->SetBound(cameraTL, cameraRB);
	}
	else if (kirby->getKirbyLocation() == LEVEL2) {
		vector<class Level*> levels = world->GetLevels();

		Vector3 levelLT = levels[1]->GetLT();
		Vector3 levelRB = levels[1]->GetRB();
		//BR = BottomRight

		Vector3 cameraTL = Vector3(levelLT.x, levelLT.y - WinMaxHeight, 0);
		Vector3 cameraRB = Vector3(levelRB.x - WinMaxWidth, levelRB.y, 0);

		Camera::Get()->SetBound(cameraTL, cameraRB);
	}
	else if (kirby->getKirbyLocation() == LEVEL3) {
		vector<class Level*> levels = world->GetLevels();

		Vector3 levelLT = levels[2]->GetLT();
		Vector3 levelRB = levels[2]->GetRB();
		//BR = BottomRight

		Vector3 cameraTL = Vector3(levelLT.x, levelLT.y - WinMaxHeight, 0);
		Vector3 cameraRB = Vector3(levelRB.x - WinMaxWidth, levelRB.y, 0);

		Camera::Get()->SetBound(cameraTL, cameraRB);
	}
	else if (kirby->getKirbyLocation() == BOSS) {
		vector<class Level*> levels = world->GetLevels();

		Vector3 levelLT = levels[3]->GetLT();
		Vector3 levelRB = levels[3]->GetRB();
		//BR = BottomRight

		Vector3 cameraTL = Vector3(levelLT.x, levelLT.y - WinMaxHeight, 0);
		Vector3 cameraRB = Vector3(levelRB.x - WinMaxWidth, levelRB.y, 0);

		Camera::Get()->SetBound(cameraTL, cameraRB);
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
			kirbyPos.x = worldLT.x - kirbySize.x / 2 + 1;
			kirby->SetPosition(kirbyPos);
			kirby->SetHitRight(true);
		}
		else {
			// Left collision
			kirbyPos.x = worldRB.x + kirbySize.x / 2 - 1;
			kirby->SetPosition(kirbyPos);
			kirby->SetHitLeft(true);
		}
	}
	else
	{
		if (kirbyLT.y > worldLT.y) {
			// Down collision
			kirbyPos.y = worldLT.y + kirbySize.y / 2 - 1;
			kirby->SetPosition(kirbyPos);
			kirby->SetHitGround(true);
		}
		else {
			// Up collision
			kirbyPos.y = worldRB.y - kirbySize.y / 2;
			kirby->SetPosition(kirbyPos);
		}
	}
}

void KirbyGame::Sound()
{
	bool hitGround = kirby->GetHitGround();
	if (kirbyPrevLocation == 3 && kirbyLocation == 4) {//boss stage
		Sounds::Get()->Pause(sounds[2]);
		Sounds::Get()->Play(sounds[1], volume);
	}
	else if (kirbyPrevLocation < 3 && kirbyLocation == 4) {//boss stage
		Sounds::Get()->Pause(sounds[0]);
		Sounds::Get()->Play(sounds[1], volume);
	}
	else if (kirbyPrevLocation == 4 && kirbyLocation < 3) {//world, level1, level2
		Sounds::Get()->Pause(sounds[1]);
		Sounds::Get()->Play(sounds[0], volume);
	}
	else if (kirbyPrevLocation == 3 && kirbyLocation < 3) {//world, level1, level2
		Sounds::Get()->Pause(sounds[2]);
		Sounds::Get()->Play(sounds[0], volume);
	}
	else if (kirbyPrevLocation == 2 && kirbyLocation == 3) {//level3
		Sounds::Get()->Pause(sounds[0]);
		Sounds::Get()->Play(sounds[2], volume);
	}

	if (kirbyPrevState != jump && kirbyCurState == jump && hitGround) {
		Sounds::Get()->Play(sounds[4], volume);
		jumpSoundTime = Time::Get()->Running();
	}
	else if (Time::Get()->Running() - jumpSoundTime > 0.09f) {
		Sounds::Get()->Pause(sounds[4]);
	}
	
	if (kirbyPrevState == inhale && kirbyCurState == inhaling) {
		Sounds::Get()->Play(sounds[5], volume);
		swallowSoundTime = Time::Get()->Running();
	}
	else if (kirbyCurState != swallowing  && kirbyCurState != inhaling &&
		Sounds::Get()->IsPlaying(sounds[5]) &&
		Time::Get()->Running() - swallowSoundTime > 0.6f) {

		Sounds::Get()->Pause(sounds[5]);
	}
	if ((kirbyPrevState == eatidle || kirbyPrevState == eatandwalk) && kirbyCurState == attacking) {
		Sounds::Get()->Play(sounds[13], volume);
		throwStarSoundTime = Time::Get()->Running();
	}
	else if (Sounds::Get()->IsPlaying(sounds[13]) &&
		Time::Get()->Running() - throwStarSoundTime > 0.28f) {
		Sounds::Get()->Pause(sounds[13]);
	}

	if ((kirbyPrevState != sandwiched && kirbyCurState == sandwiched) ||
		((kirbyPrevState == jumpdown || kirbyPrevState == falldown) && 
		kirbyCurState == flatten)) {
		Sounds::Get()->Play(sounds[6], volume);
		sandwichedSoundTime = Time::Get()->Running();
	}
	else if (Sounds::Get()->IsPlaying(sounds[6]) &&
		Time::Get()->Running() - sandwichedSoundTime > 0.12f) {
		Sounds::Get()->Pause(sounds[6]);
	}


	if (hitGround && kirbyCurState != inhaled &&
		kirbyCurState != inhaling && kirbyCurState != eatidle &&
		kirbyCurState != eatandwalk && Keyboard::Get()->Down(VK_DOWN)) {
		Sounds::Get()->Play(sounds[11], volume);
		headdownSoundTime = Time::Get()->Running();
	}
	else if (Sounds::Get()->IsPlaying(sounds[11]) &&
		Time::Get()->Running() - headdownSoundTime > 0.071f) {
		Sounds::Get()->Pause(sounds[11]);
	}

	if (kirbyPrevState!= hitEnemy && kirbyCurState == hitEnemy) {
		Sounds::Get()->Play(sounds[7], volume);
		hitSoundTime = Time::Get()->Running();
	}
	else if (Sounds::Get()->IsPlaying(sounds[7]) &&
		Time::Get()->Running() - hitSoundTime > 0.4f) {
		Sounds::Get()->Pause(sounds[7]);
	}

	if (kirbyPrevState != flyup && kirbyCurState == flyup) {
		Sounds::Get()->Play(sounds[8], volume);
		flyupSoundTime = Time::Get()->Running();
	}
	else if (Sounds::Get()->IsPlaying(sounds[8]) &&
		Time::Get()->Running() - flyupSoundTime > 0.16f) {
		Sounds::Get()->Pause(sounds[8]);
	}

	if (kirbyPrevState != exhaling && kirbyCurState == exhaling) {
		Sounds::Get()->Play(sounds[9], volume);
		exhaleSoundTime = Time::Get()->Running();
	}
	else if (Sounds::Get()->IsPlaying(sounds[9]) &&
		Time::Get()->Running() - exhaleSoundTime > 0.16f) {
		Sounds::Get()->Pause(sounds[9]);
	}

	prevAbilityUse = abilityUse;
	abilityUse = kirby->GetAbilityUse();//get ability
	if (!prevAbilityUse && abilityUse) {
		Sounds::Get()->Play(sounds[12], volume);
		getAbilitySoundTime = Time::Get()->Running();
	}
	else if (Sounds::Get()->IsPlaying(sounds[12]) &&
		Time::Get()->Running() - getAbilitySoundTime > 0.98f) {
		Sounds::Get()->Pause(sounds[12]);
	}

	if (deathCnt > prevDeathCnt && kirbyCurState != swallowing) {
		Sounds::Get()->Play(sounds[14], volume);
		enemykillSoundTime = Time::Get()->Running();
	}
	else if(Time::Get()->Running() - enemykillSoundTime > 0.17f){
		Sounds::Get()->Pause(sounds[14]);
	}

	if (kirbyPrevState != drift && kirbyCurState == drift) {
		Sounds::Get()->Play(sounds[10], volume);
		endDashSoundTime = Time::Get()->Running();
	}
	else if (Time::Get()->Running() - endDashSoundTime > 0.25f) {
		Sounds::Get()->Pause(sounds[10]);
	}
}

void KirbyGame::UpdateEffect()
{
	//start kirby Effect of inhaling
	effects[0]->SetKirbyPos(kirby->GetPosition(), kirby->GetLeft());
	effects[0]->UpdateEatEffect();
	effects[1]->UpdateSwallowEffect();
	effects[2]->UpdateBlowStarEffect(Time::Get()->Delta());
	effects[3]->UpdateExplodeOnEnemy();
	effects[4]->UpdateHitEffect(Time::Get()->Delta());
	effects[5]->UpdateBlowAir(Time::Get()->Delta());
}

void KirbyGame::SetThrowStar()
{
	effects[2]->SetKirbyPos(kirby->GetPosition(), kirby->GetLeft());
	effects[2]->SetKirbyBlowStar();
	//set duration for this effect
	effects[2]->StartTimer(10.0f);
}

void KirbyGame::SetKirbyEat()
{
	effects[0]->SetKirbyPos(kirby->GetPosition(), kirby->GetLeft());
	effects[0]->SetKirbyEat();
	effects[0]->StartTimer(100000.0f);
}

void KirbyGame::SetKirbyBlowAir()
{
	effects[5]->SetKirbyPos(kirby->GetPosition(), kirby->GetLeft());
	effects[5]->SetKirbyBlowAir();
	effects[5]->StartTimer(2.0f);
}

bool KirbyGame::KirbyCollisionWithWorld(BoundingBox* kirbyBox, Rect* worldRect)
{
	BoundingBox* worldBox = nullptr;
	worldBox = worldRect->GetBox();
	Vector3 kirbyPos = kirby->GetPosition();

	//if there is collision
	if (BoundingBox::OBB(kirbyBox, worldBox)) {
		//world->SetColor(i, Values::Blue);
		float rotation = worldRect->GetRotation();
		//when hit with slope calculate y value of that slope

		Vector3 dir = kirby->GetDirection();

		if (rotation > 0.1f) {//upper
			int idx = CheckSlopeRange(kirbyPos.x);
			int state = kirby->GetState();
			if (idx > -1 && state != jump && !(state == inhaled && dir.y > 0.2f)) {
				SetKirbyPosForSlope(idx, kirbyPos, rotation);
			}
		}
		else if (rotation < -0.1f) {//down
			int idx = CheckSlopeRange(kirbyPos.x);
			int state = kirby->GetState();
			if (idx > -1 && state != jump && !(state == inhaled && dir.y > 0.2f)) {
				SetKirbyPosForSlope(idx, kirbyPos, rotation);
			}
		}
		else {
			FixKirbyPosition(worldRect);
		}
	}
	return true;
}

int KirbyGame::CheckSlopeRange(float positionX)
{
	for (int i = 0; i < slopeRange.size(); i++) {
		if (slopeRange[i].first <= positionX && positionX <= slopeRange[i].second) {
			return i;
		}
	}

	return -1;
}

void KirbyGame::SetKirbyPosForSlope(int idx, Vector3 kirbyPos, float rotation)
{
	switch (idx)
	{
	case 0:
		kirbyPos.y = floor + (kirbyPos.x - slopeRange[0].first);
		break;
	case 1:
		kirbyPos.y = floor2 + (kirbyPos.x - slopeRange[1].first) * tan27;
		break;
	case 2:
		kirbyPos.y = floor2 + (slopeRange[2].second - kirbyPos.x) * tan27;
		break;
	case 3:
		kirbyPos.y = floor + (slopeRange[3].second - kirbyPos.x);
		break;
	case 4:
		kirbyPos.y = floor + (kirbyPos.x - slopeRange[4].first) * tan27;
		break;
	case 5:
		kirbyPos.y = floor + (slopeRange[5].second - kirbyPos.x) * tan27;
		break;
	default:
		break;
	}
	kirby->SetHitGround(true);
	kirby->SetPosition(kirbyPos);
}

void KirbyGame::SetEnemyPosForSlope(int idx, Vector3 enemyPos, float rotation, class Enemy* enemy)
{
	switch (idx)
	{
	case 0:
		enemyPos.y = floor + (enemyPos.x - slopeRange[0].first);
		break;
	case 1:
		enemyPos.y = floor2 + (enemyPos.x - slopeRange[1].first) * tan27;
		break;
	case 2:
		enemyPos.y = floor2 + (slopeRange[2].second - enemyPos.x) * tan27;
		break;
	case 3:
		enemyPos.y = floor + (slopeRange[3].second - enemyPos.x);
		break;
	case 4:
		enemyPos.y = floor + (enemyPos.x - slopeRange[4].first) * tan27;
		break;
	case 5:
		enemyPos.y = floor + (slopeRange[5].second - enemyPos.x) * tan27;
		break;
	default:
		break;
	}
	enemy->SetHitGround(true);
	enemy->SetPosition(enemyPos);
}

void KirbyGame::CheckInhaleEnemy(int kirbyLocation, vector<Level*> levels)
{
	//vector that stores inhaled enemy
	vector<pair<wstring, Vector3>> enemySwallowed;

	//get enemies in current level
	vector<Enemy*> enemies = levels[kirbyLocation]->GetEnemies();
	
	//get kirby position
	Vector3 kirbyPos = kirby->GetPosition();

	//get kirby facing direction
	bool left = kirby->GetLeft();

	for (int i = 0; i < enemies.size(); i++) {
		Vector3 enemyPos = enemies[i]->GetPosition();
		//get x distance between kirby and enemy
		int distanceX = abs(enemyPos.x - kirbyPos.x);
		//get y distance between kirby and enemy
		int distanceY = abs(enemyPos.y - kirbyPos.y);
		if (enemies[i]->GetState() != 3 && 
			(distanceX < 200.0f && distanceY < 120.0f) &&
			((left && kirbyPos.x > enemyPos.x) || 
			(!left && kirbyPos.x <= enemyPos.x))) {
			//push back swallowed enemy info
			int frameIdx = enemies[i]->GetAnimator()->GetCurrentFrameIndex();

			wstring path = TexturePath + 
						String::ToWString(enemies[i]->GetName() + to_string(frameIdx)) +
						L".png";
			//cout << enemies[i]->GetName() << frameIdx << "\n";
			enemySwallowed.push_back({ path, enemies[i]->GetPosition() });

			//erase spark and beam effect
			if (enemies[i]->GetSparkEffectRect()) {
				enemies[i]->StopSparkEffect();
				kirby->SetAbility(Ability::spark);
			}
			else if (enemies[i]->GetBeamEffectRect()) {
				enemies[i]->StopBeamEffect();
				kirby->SetAbility(Ability::beam);
			}
			else {
				kirby->SetAbility(Ability::none);
			}
			//set ability for kirby
			if (enemies[i]->GetAbility().compare("spark") == 0) {
				kirby->SetAbility(Ability::spark);
			}
			else if (enemies[i]->GetAbility().compare("beam") == 0) {
				kirby->SetAbility(Ability::beam);
			}
			else {
				kirby->SetAbility(Ability::none);
			}
			enemies[i]->SetDeathStart();
		}
	}
	//change kirby state to swallowing
	//all other kirby states are not allowed during this state
	//move enemy through bezier curve
	if (enemySwallowed.size()) {
		effects[1]->SetKirbyPos(kirby->GetPosition(), kirby->GetLeft());
		if (kirbyLocation == 3) {
			effects[1]->SetKirbySwallow(enemySwallowed, { 64.0f, 64.0f, 0.0f });
		}
		else {
			effects[1]->SetKirbySwallow(enemySwallowed, { 128.0f, 128.0f, 0.0f });
		}

		effects[1]->StartTimer(2.0f);
		kirby->SetState(swallowing);
	}
}

void KirbyGame::CheckHitByEnemy(vector<Enemy*> enemies, int idx)
{
	//get kirby position
	Vector3 kirbyPos = kirby->GetPosition();

	float invulnerableTime = Time::Get()->Running() - kirby->GetHitEnemy();
	//check if enemy attack effect hit kirby
	if (enemies[idx]->GetState() < 3) {//if enemy is not dead
		Rect* attackEffectRect = nullptr;
		//cout << enemies[j]->GetName() << "\n";
		string enemyName = enemies[idx]->GetName();
		if (enemyName.compare("waddledoo") == 0) {
			attackEffectRect = enemies[idx]->GetBeamEffectRect();
		}
		else if (enemyName.compare("sparky") == 0) {
			attackEffectRect = enemies[idx]->GetSparkEffectRect();
		}

		BoundingBox* attackBoundingBox = nullptr;
		if (attackEffectRect) {
			attackBoundingBox = attackEffectRect->GetBox();
		}
		BoundingBox* kirbyBox = kirby->GetRect()->GetBox();
		//if enemy attack hit kirby
		if (attackBoundingBox && invulnerableTime > invincibleDuration &&
			BoundingBox::OBB(kirbyBox, attackBoundingBox)) {
			EnemyAttackCollideKirby(attackEffectRect);
		}
	}
	enemyBox = GetEnemyBox(enemies, idx);
	BoundingBox* kirbyBox = kirby->GetRect()->GetBox();

	//check if kirby collides with enemy
	if (enemyBox && invulnerableTime > invincibleDuration && 
		BoundingBox::OBB(kirbyBox, enemyBox)) {
		effects[4]->SetKirbyPos(kirbyPos, kirby->GetLeft());
		effects[4]->SetHitEffect();
		effects[4]->StartTimer(0.5f);//set duration of effect

		//while kirby was hit while inhaling
		if (kirby->GetState() == inhaling) {
			effects[0]->StartTimer(0.1f);//remove enemy inhaling effect
			if (effects[1]->isTimerSet()) {//if enemy was being pulled
				effects[1]->StartTimer(0.1f);//remove enemy pulling effect
			}
		}

		kirby->SetState(hitEnemy);
		kirby->SetHitEnemy();
	}
}

void KirbyGame::EnemyAttackCollideKirby(Rect* effect)

{
	pair<Vector3, Vector3> intersection;
	IntersectRect(kirby->GetRect(), effect, intersection);

	Vector3 kirbyPos = kirby->GetRect()->GetPosition();
	Vector3 kirbySize = kirby->GetRect()->GetSize();

	Vector3 kirbyLT = kirby->GetRect()->GetLT();
	Vector3 kirbyRB = kirby->GetRect()->GetRB();

	Vector3 effectRectPos = effect->GetPosition();
	Vector3 effectLT = effect->GetLT();
	Vector3 effectRB = effect->GetRB();

	Vector3 tmp = kirbyPos - effectRectPos;
	if (intersection.first == Values::ZeroVec3 &&
		intersection.second == Values::ZeroVec3) {
		return;
	}
	// Determine the collision direction based on the intersection size
	if (intersection.first.x - intersection.second.x >
		intersection.second.y - intersection.first.y) {
		if (kirbyLT.x < effectLT.x) {
			// Right collision
			kirbyPos.x = effectLT.x - kirbySize.x / 2 + 1;
			//kirby->SetPosition(kirbyPos);
			kirby->SetEffectHit(1);//right hit
		}
		else {
			// Left collision
			kirbyPos.x = effectRB.x + kirbySize.x / 2 - 1;
			//kirby->SetPosition(kirbyPos);
			kirby->SetEffectHit(2);//left hit
		}
	}
	else
	{
		if (kirbyLT.y > effectLT.y) {
			// Down collision
			kirbyPos.y = effectLT.y + kirbySize.y / 2 - 1;
			//kirby->SetPosition(kirbyPos);
			kirby->SetEffectHit(3);//down hit
		}
		else {
			// Up collision
			kirbyPos.y = effectRB.y - kirbySize.y / 2;
		}
	}

	//while kirby was hit while inhaling
	if (kirby->GetState() == inhaling) {
		effects[0]->StartTimer(0.1f);//remove enemy inhaling effect
		if (effects[1]->isTimerSet()) {//if enemy was being pulled
			effects[1]->StartTimer(0.1f);//remove enemy pulling effect
		}
	}
	kirby->SetState(hitEnemy);
	kirby->SetHitEnemy();
}

void KirbyGame::EnemyAndKirby(int kirbyLocation, vector<class Level*> levels)
{
	//get enemies in current level
	vector<Enemy*> enemies = levels[kirbyLocation]->GetEnemies();
	for (size_t i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->GetState() != 3) {//do not check dead enemy
			//is kirby hit by enemy?
			CheckHitByEnemy(enemies, i);
			//check if kirby blow air collides with enemy
			CheckBlowAirHitEnemy(enemies, i);
			//check star hit enemy
			CheckStarHitEnemy(enemies, i);

			KillEnemyWithEffect(enemies, i);
		}
	}
}

void KirbyGame::BossAndKirby(int kirbyLocation, vector<class Level*> levels)
{
	//check ending box collision
	BoundingBox* endingBox = nullptr;
	Rect* endingRect = levels[kirbyLocation]->GetEndingRect();
	if (endingRect) {
		endingBox = endingRect->GetBox();
	}
	if (endingBox && BoundingBox::OBB(endingBox, kirby->GetRect()->GetBox())) {
		showEnding = true;
	}

	//get apples in current level
	vector<Enemy*> apples = levels[kirbyLocation]->GetEnemies();
	Rect* bossRect = levels[kirbyLocation]->GetBossRect();
	BoundingBox* bossBox = nullptr;
	if (bossRect) {
		bossBox = bossRect->GetBox();
	}
	else {
		return;
	}
	float invulnerableTime = Time::Get()->Running() - kirby->GetHitEnemy();
	
	if (invulnerableTime > invincibleDuration &&
		levels[kirbyLocation]->BossBlowHitKirby(kirby->GetRect()->GetBox()) > -1) {
		effects[4]->SetKirbyPos(kirby->GetPosition(), kirby->GetLeft());
		effects[4]->SetHitEffect();
		effects[4]->StartTimer(0.5f);//set duration of effect

		//while kirby was hit while inhaling
		if (kirby->GetState() == inhaling) {
			effects[0]->StartTimer(0.1f);//remove enemy inhaling effect
			if (effects[1]->isTimerSet()) {
				effects[1]->StartTimer(0.1f);//remove enemy pulling effect
			}
		}

		kirby->SetState(hitEnemy);
		kirby->SetHitEnemy();
	}

	CheckStarHitBoss(bossRect, kirbyLocation, levels);
	CheckAbilityHitBoss(bossRect, kirbyLocation, levels);

	//if kirby collides with boss
	if (invulnerableTime > invincibleDuration && 
		BoundingBox::OBB(bossBox, kirby->GetRect()->GetBox())) {
		effects[4]->SetKirbyPos(kirby->GetPosition(), kirby->GetLeft());
		effects[4]->SetHitEffect();
		effects[4]->StartTimer(0.5f);//set duration of effect

		//while kirby was hit while inhaling
		if (kirby->GetState() == inhaling) {
			effects[0]->StartTimer(0.1f);//remove enemy inhaling effect
			if (effects[1]->isTimerSet()) {
				effects[1]->StartTimer(0.1f);//remove enemy pulling effect
			}
		}
		
		kirby->SetState(hitEnemy);
		kirby->SetHitEnemy();
	}

}

void KirbyGame::CheckBlowAirHitEnemy(vector<Enemy*> enemies, int idx)
{		
	//check if kirby blow air collides with enemy
	effectBox = nullptr;
	effectRect = effects[5]->GetRect();
	if (effectRect) {
		effectBox = effectRect->GetBox();
	}
	BoundingBox* enemyBox = GetEnemyBox(enemies, idx);

	if (effectBox && enemyBox && BoundingBox::OBB(effectBox, enemyBox)) {
		effects[5]->StartTimer(0.1f);
		enemies[idx]->SetDeathStart();//start enemy death timer
	}
}

void KirbyGame::CheckStarHitEnemy(vector<Enemy*> enemies, int idx)
{
	effectBox = nullptr;
	effectRect = effects[2]->GetRect();
	if (effectRect) {
		effectBox = effectRect->GetBox();
	}
	BoundingBox* enemyBox = GetEnemyBox(enemies, idx);
	//check if big star that kirby throws hit enemy
	if (effectBox && enemyBox && BoundingBox::OBB(effectBox, enemyBox)) {
		effects[3]->SetKirbyStarExplodeOnEnemy(enemies[idx]->GetPosition());
		effects[3]->StartTimer(0.2f);

		if (effects[2]->isTimerSet()) {
			effects[2]->StartTimer(0.0f);
		}
		enemies[idx]->SetDeathStart();//start enemy death timer
	}
}

void KirbyGame::CheckStarHitBoss(Rect* bossRect, int kirbyLocation, vector<class Level*> levels)
{
	//if boss has hit or death state
	if (levels[kirbyLocation]->GetBossState() >= 3 ||
		Time::Get()->Running() - bossHitTime < bossInvulnerableTime) {
		return;
	}
	effectBox = nullptr;
	effectRect = effects[2]->GetRect();
	if (effectRect) {
		effectBox = effectRect->GetBox();
	}

	BoundingBox* bossBox = nullptr;
	if (bossRect) {
		bossBox = bossRect->GetBox();
	}
	//check if big star that kirby throws hit enemy
	if (effectBox && enemyBox && BoundingBox::OBB(effectBox, bossBox)) {
		effects[3]->SetKirbyStarExplodeOnEnemy(bossRect->GetPosition());
		effects[3]->StartTimer(0.2f);
		if (effects[2]->isTimerSet()) {
			effects[2]->StartTimer(0.0f);
		}

		//save current state of boss for later use
		levels[kirbyLocation]->SaveBossState();
		//save time when boss is hit by kirby
		levels[kirbyLocation]->SetBossHitTimer();
		//get health of boss
		int bossHealth = levels[kirbyLocation]->GetBossHealth();
		//set health of boss
		levels[kirbyLocation]->SetBossHealth(bossHealth - dmg);
		//set boss state to 'hit' == 3
		levels[kirbyLocation]->SetBossState(3);
		bossHitTime = Time::Get()->Running();
	}
}

void KirbyGame::CheckAbilityHitBoss(Rect* bossRect, int kirbyLocation, vector<class Level*> levels)
{
	//if boss has hit or death state or invulnerable
	if (levels[kirbyLocation]->GetBossState() >= 3 || 
		Time::Get()->Running() - bossHitTime < bossInvulnerableTime) {
		return;
	}
	KirbyEffect* sparkEffect = kirby->GetSparkEffect();
	KirbyEffect* beamEffect = kirby->GetBeamEffect();
	//if kirby is using spark or beam
	if (sparkEffect->isTimerSet()) {
		effectRect = sparkEffect->GetRect();
	}
	else if (beamEffect->isTimerSet()) {
		effectRect = beamEffect->GetRect();
	}
	else {
		return;
	}

	effectBox = nullptr;
	if (effectRect) {
		effectBox = effectRect->GetBox();
	}

	BoundingBox* bossBox = nullptr;
	if (bossRect) {
		bossBox = bossRect->GetBox();
	}
	//check if beam or spark hit boss
	if (effectBox && enemyBox && BoundingBox::OBB(effectBox, bossBox)) {
		effects[3]->SetKirbyStarExplodeOnEnemy(bossRect->GetPosition());
		effects[3]->StartTimer(0.2f);
		if (effects[2]->isTimerSet()) {
			effects[2]->StartTimer(0.1f);
		}
		
		//save current state of boss for later use
		levels[kirbyLocation]->SaveBossState();
		//save time when boss is hit by kirby
		levels[kirbyLocation]->SetBossHitTimer();
		//get health of boss
		int bossHealth = levels[kirbyLocation]->GetBossHealth();
		//set health of boss
		levels[kirbyLocation]->SetBossHealth(bossHealth - 2);
		//set boss state to 'hit' == 3
		levels[kirbyLocation]->SetBossState(3);
		bossHitTime = Time::Get()->Running();
	}
}

void KirbyGame::KillEnemyWithEffect(vector<Enemy*> enemies, int idx)
{
	//check if kirby kills enemy with effect
	KirbyEffect* beam = kirby->GetBeamEffect();
	KirbyEffect* spark = kirby->GetSparkEffect();
	Rect* beamSparkRect = nullptr;
	BoundingBox* beamSparkBox = nullptr;

	//if effects are alive
	if (beam->isTimerSet()) {
		beamSparkRect = beam->GetRect();
	}
	else if (spark->isTimerSet()) {
		beamSparkRect = spark->GetRect();
	}
	//get bounding box
	if (beamSparkRect) {
		beamSparkBox = beamSparkRect->GetBox();
	}
	//check if hits enemy
	if (beamSparkBox && enemyBox && BoundingBox::OBB(beamSparkBox, enemyBox)) {
		effects[3]->SetKirbyStarExplodeOnEnemy(enemies[idx]->GetPosition());
		effects[3]->StartTimer(0.2f);

		enemies[idx]->SetDeathStart();//start enemy death timer
	}
}

BoundingBox* KirbyGame::GetEnemyBox(vector<Enemy*> enemies, int idx)
{
	enemyBox = nullptr;
	enemyRect = enemies[idx]->GetRect();
	if (enemyRect) {
		enemyBox = enemyRect->GetBox();
	}
	return enemyBox;
}

void KirbyGame::AddSounds()
{
	for (size_t i = 0; i < sounds.size(); i++)
	{
		Sounds::Get()->AddSound(sounds[i], 
			SoundPath + String::ToWString(sounds[i]), true);
	}
}
