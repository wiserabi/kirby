#include "stdafx.h"
#include "KirbyGame.h"

#include "Geomatries/AnimationRect.h"
#include "KirbyCharacter.h"
#include "World.h"
#include "UI/HUD.h"
#include "Geomatries/Rect.h"
#include "Enemy.h"
#include "EnemyInfo.h"
#include "KirbyEffect.h"

#define SWALLOWRANGE 200.0f

void KirbyGame::Init()
{
	kirby = new KirbyCharacter({ 720, 360, 1.0 }, { 128, 128, 1 });
	Sounds::Get()->AddSound("Vegetable-Valley.mp3",
		SoundPath + L"Vegetable-Valley.mp3", true);
	Sounds::Get()->Play("Vegetable-Valley.mp3", 0.1f);

	hud = new HUD();
	world = new World();
	enemyInfo = new EnemyInfo();

	enemies.push_back(new Enemy({ 1000, 430, 0 }, { 128, 128, 1 }, "waddledee", enemyInfo));
	enemies.push_back(new Enemy({ 1000, 430, 0 }, { 128, 128, 1 }, "waddledee", enemyInfo));

	enemies.push_back(new Enemy({ 500, 430, 0 }, { 128, 128, 1 }, "sparky", enemyInfo));
	enemies.push_back(new Enemy({ 500, 430, 0 }, { 128, 128, 1 }, "sparky", enemyInfo));
	
	enemies.push_back(new Enemy({ 700, 430, 0 }, { 128, 128, 1 }, "waddledee", enemyInfo));
	enemies.push_back(new Enemy({ 700, 430, 0 }, { 128, 128, 1 }, "waddledee", enemyInfo));
	
	enemies.push_back(new Enemy({ 800, 430, 0 }, { 128, 128, 1 }, "waddledoo", enemyInfo));
	enemies.push_back(new Enemy({ 800, 430, 0 }, { 128, 128, 1 }, "waddledoo", enemyInfo));


	effects.push_back(new KirbyEffect());//used for kirby inhaling
	effects.push_back(new KirbyEffect());//pulling enemy as effect
	effects.push_back(new KirbyEffect());//attacking
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

	for (int i = 0; i < enemies.size(); i++) {
		SAFE_DELETE(enemies[i]);
	}
	enemies.clear();
	SAFE_DELETE(enemyInfo);
	SAFE_DELETE(kirby);
	SAFE_DELETE(hud);
	SAFE_DELETE(world);
}

void KirbyGame::Update()
{
	CheckReset();

	Sound();
	world->Update();

	for (class Enemy* enemy : enemies) {
		enemy->SetKirbyPos(kirby->GetPosition());
		enemy->Update();
	}

	kirby->Move();
	kirby->Update();

	//check if there is a timer set for animation
	UpdateEffect();

	State kirbyCurState = kirby->GetState();
	State kirbyPrevState = kirby->GetPrevState();

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
	//check if kirby is inhaling
	else if (kirbyCurState == inhaling) {
		CheckInhaleEnemy();
	}
	//when changed to throw star
	else if ((kirbyPrevState == eatidle || kirbyPrevState == eatandwalk) && kirbyCurState == attacking) {
		SetThrowStar();
	}
	
	kirby->SetHitGround(false);
	kirby->SetHitLeft(false);
	kirby->SetHitRight(false);

	for (size_t j = 0; j < enemies.size(); j++) {
		enemies[j]->SetHitGround(false);
		enemies[j]->SetHitLeft(false);
		enemies[j]->SetHitRight(false);
	}

	//if kirby is in the world
	if (kirby->isKirbyInWorld()) {
		BoundingBox* kirbyBox = kirby->GetRect()->GetBox();
		vector<Rect*> worldRects = world->GetRects();
		for (size_t i = 0; i < worldRects.size(); i++) {
			KirbyCollisionWithWorld(kirbyBox, worldRects[i]);

			EnemyCollisions(enemies, worldRects[i], kirbyBox);

			BoundingBox* effectBox = nullptr;
			Rect* effectRect = effects[2]->GetRect();
			if (effectRect) {
				effectBox = effectRect->GetBox();
			}
			//check if big star thrown by kirby hits the wall
			if (effectRect && effectBox && BoundingBox::OBB(effectBox, worldRects[i]->GetBox())) {
				effects[2]->StopEffect();
			}
		}
		
		for (size_t i = 0; i < enemies.size(); i++)
		{
			if(enemies[i]->CheckDeath()) {
				enemies.erase(enemies.begin() + i);
				i--;
			}
		}
	}

	hud->Update();
	SetCameraBound();
}

void KirbyGame::Render()
{
	world->Render();
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->Render();
	}

	for (size_t i = 0; i < effects.size(); i++)
	{
		effects[i]->RenderEffect();
	}
	effects[1]->RenderSwallowEffect(enemySwallowed);
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

	if (kirby->isKirbyInWorld()) {
		Vector3 worldPos = world->getWorldPosition();//TL = TopLeft
		Vector3 worldSize = world->getWorldSize();
		Vector3 worldTL = Vector3(worldPos.x - worldSize.x / 2, worldPos.y + worldSize.y / 2, 0);
		Vector3 worldBR = Vector3(worldPos.x + worldSize.x / 2, worldPos.y - worldSize.y / 2, 0);
		//BR = BottomRight
		
		Vector3 cameraTL = Vector3(worldTL.x, worldTL.y - WinMaxHeight, 0);
		Vector3 cameraBR = Vector3(worldBR.x - WinMaxWidth, worldBR.y, 0);

		Camera::Get()->SetBound(cameraTL, cameraBR);
	}
	else {
		//get level pos
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

void KirbyGame::FixEnemyPosition(Rect* worldRect, int idx)
{
	pair<Vector3, Vector3> intersection;
	IntersectRect(enemies[idx]->GetRect(), worldRect, intersection);

	Vector3 enemyPos = enemies[idx]->GetPosition();
	Vector3 enemySize = enemies[idx]->GetRect()->GetSize();

	Vector3 enemyLT = enemies[idx]->GetLT();
	Vector3 enemyRB = enemies[idx]->GetRB();

	Vector3 worldRectPos = worldRect->GetPosition();
	Vector3 worldLT = worldRect->GetLT();
	Vector3 worldRB = worldRect->GetRB();

	Vector3 tmp = enemyPos - worldRectPos;
	if (intersection.first == Values::ZeroVec3 &&
		intersection.second == Values::ZeroVec3) {
		return;
	}
	// Determine the collision direction based on the intersection size
	if (intersection.first.x - intersection.second.x >
		intersection.second.y - intersection.first.y) {
		if (enemyLT.x < worldLT.x) {
			// Right collision
			enemyPos.x = worldLT.x - enemySize.x / 2 + 1;
			enemies[idx]->SetPosition(enemyPos);
			enemies[idx]->SetHitRight(true);
		}
		else {
			// Left collision
			enemyPos.x = worldRB.x + enemySize.x / 2 - 1;
			enemies[idx]->SetPosition(enemyPos);
			enemies[idx]->SetHitLeft(true);
		}
	}
	else
	{
		if (enemyLT.y > worldLT.y) {
			// Down collision
			enemyPos.y = worldLT.y + enemySize.y / 2 - 1;
			enemies[idx]->SetPosition(enemyPos);
			enemies[idx]->SetHitGround(true);
		}
		else {
			// Up collision
			enemyPos.y = worldRB.y - enemySize.y / 2;
			enemies[idx]->SetPosition(enemyPos);
		}
	}
}

void KirbyGame::CheckReset()
{
	if (Keyboard::Get()->Down('R')) {
		Sounds::Get()->Pause("Vegetable-Valley.mp3");
		vector<class Enemy*>().swap(enemies);

		KirbyGame::Init();
	}
}

void KirbyGame::Sound()
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
}

void KirbyGame::UpdateEffect()
{
	//start kirby Effect of inhaling
	effects[0]->SetKirbyPos(kirby->GetPosition(), kirby->GetLeft());
	effects[0]->UpdateEffect(Time::Get()->Delta());
	if (kirby->GetState() == swallowing) {
		//start kirby Effect of swallowing enemy
		effects[1]->SetKirbyPos(kirby->GetPosition(), kirby->GetLeft());

		//finish when all enemies reach p1 or timer goes out
		if (effects[1]->UpdateSwallowEffect(enemySwallowed)) {
			//kirby idle motion after eat enemy
			kirby->SetAttackDelay();
			kirby->SetState(eatidle);
			effects[1]->StartTimer(0.1f);
			CheckAbility();
			vector<pair<Enemy*, int>>().swap(enemySwallowed);
			effects[0]->StartTimer(0.1f);
		}
	}
	effects[2]->UpdateEffect(Time::Get()->Delta());
	effects[3]->UpdateEffect(Time::Get()->Delta());
	effects[4]->UpdateHitEffect(Time::Get()->Delta());
	effects[5]->UpdateBlowAir(Time::Get()->Delta());
}

void KirbyGame::CheckInhaleEnemy()
{
	//cout << "kirby is inhaling!" << "\n";
	Vector3 kirbyPos = kirby->GetPosition();

	//check whether kirby is facing right or left
	if (kirby->GetLeft()) {
		//check all enemies on left
		for (int i = 0; i < enemies.size(); i++) {
			Enemy* enemy = enemies[i];
			Vector3 pos = enemy->GetPosition();
			//check enemy in inhaling range, if enemy is not dead
			if (enemy->GetState() != 3 && pos.y <= kirbyPos.y + 120.0f && pos.y >= kirbyPos.y - 120.0f &&
				pos.x <= kirbyPos.x && kirbyPos.x - SWALLOWRANGE < pos.x) {
				//cout << "Enemy is in range!" << "\n";
				int curframe = enemies[i]->GetAnimator()->GetCurrentFrameIndex();
				//erase spark and beam effect
				if (enemies[i]->GetBeamEffectRect()) {
					enemies[i]->StopBeamEffect();
				}
				else if (enemies[i]->GetSparkEffectRect()) {
					enemies[i]->StopSparkEffect();
				}

				enemySwallowed.push_back({ enemies[i] , curframe });
				enemies.erase(enemies.begin() + i);
				i--;
			}
		}
	}
	else {
		//check all enemies on right
		for (int i = 0; i < enemies.size(); i++) {
			Enemy* enemy = enemies[i];
			Vector3 pos = enemy->GetPosition();
			//check enemy in inhaling range, if enemy is not dead
			if (enemy->GetState() != 3 && pos.y <= kirbyPos.y + 120.0f && pos.y >= kirbyPos.y - 120.0f &&
				pos.x >= kirbyPos.x && kirbyPos.x + SWALLOWRANGE > pos.x) {
				//cout << "Enemy is in range!" << "\n";
				int curframe = enemies[i]->GetAnimator()->GetCurrentFrameIndex();
				//erase spark and beam effect
				if (enemies[i]->GetBeamEffectRect()) {
					enemies[i]->StopBeamEffect();
				}
				else if (enemies[i]->GetSparkEffectRect()) {
					enemies[i]->StopSparkEffect();
				}
				enemySwallowed.push_back({ enemy, curframe });
				enemies.erase(enemies.begin() + i);
				i--;
			}
		}
	}
	//change kirby state to swallowing
	//all other kirby states are not allowed during this state
	//move enemy through bezier curve
	if (!enemySwallowed.empty()) {
		effects[1]->SetKirbyPos(kirby->GetPosition(), kirby->GetLeft());
		effects[1]->SetKirbySwallow(enemySwallowed);
		effects[1]->StartTimer(100000.0f);
		kirby->SetState(swallowing);
	}
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

void KirbyGame::KirbyCollisionWithWorld(BoundingBox* kirbyBox, Rect* worldRect)
{
	//if there is collision
	if (BoundingBox::OBB(kirbyBox, worldRect->GetBox())) {
		//world->SetColor(i, Values::Blue);
		FixKirbyPosition(worldRect);
	}
}

void KirbyGame::EnemyCollisions(vector<class Enemy*>& enemies, Rect* worldRect, BoundingBox* kirbyBox)
{
	//assume there are enemies in the world
	for (size_t j = 0; j < enemies.size(); j++)
	{
		//check enemy collision with world
		BoundingBox* enemyBox = nullptr;
		Rect* enemyRect = enemies[j]->GetRect();
		if (enemyRect) {
			enemyBox = enemyRect->GetBox();
		}

		if (enemyBox && BoundingBox::OBB(enemyBox, worldRect->GetBox())) {
			//world->SetColor(i, Values::Red);
			FixEnemyPosition(worldRect, j);
		}
		float invulnerableTime = Time::Get()->Running() - kirby->GetHitEnemy();
		//check if enemy attack effect hit kirby
		if (enemies[j]->GetState() < 3) {//if kirby is not dead
			Rect* attackEffectRect = nullptr;
			//cout << enemies[j]->GetName() << "\n";
			string enemyName = enemies[j]->GetName();
			if (enemyName.compare("waddledoo") == 0) {
				attackEffectRect = enemies[j]->GetBeamEffectRect();
			}
			else if (enemyName.compare("sparky") == 0) {
				attackEffectRect = enemies[j]->GetSparkEffectRect();
			}

			BoundingBox* attackBoundingBox = nullptr;
			if (attackEffectRect) {
				attackBoundingBox = attackEffectRect->GetBox();
			}
			//if enemy attack hit kirby
			if (attackBoundingBox && invulnerableTime > 2.0f && 
				BoundingBox::OBB(kirbyBox, attackBoundingBox)) {
				EnemyAttackCollideKirby(attackEffectRect);
			}
		}
		
		//check if kirby blow air collides with enemy
		BoundingBox* effectBox = nullptr;
		Rect* effectRect = effects[5]->GetRect();
		if (effectRect) {
			effectBox = effectRect->GetBox();
		}

		if (effectBox && enemyBox && BoundingBox::OBB(effectBox, enemyBox)) {
			effects[5]->StartTimer(0.1f);
			enemies[j]->SetDeathStart();//start enemy death timer
			continue;
		}

		effectBox = nullptr;
		effectRect = effects[2]->GetRect();
		if (effectRect) {
			effectBox = effectRect->GetBox();
		}

		//check if big star that kirby throws hit enemy
		if (effectBox && enemyBox && BoundingBox::OBB(effectBox, enemyBox)) {
			effects[3]->SetKirbyStarExplodeOnEnemy(enemies[j]->GetPosition());
			effects[3]->StartTimer(0.2f);

			effects[2]->StartTimer(0.01f);
			enemies[j]->SetDeathStart();//start enemy death timer
			continue;
		}
		
		//check if kirby collides with enemy
		if (enemyBox && invulnerableTime > 2.0f && BoundingBox::OBB(kirbyBox, enemyBox)) {
			effects[4]->SetKirbyPos(kirby->GetPosition(), kirby->GetLeft());
			effects[4]->SetHitEffect();
			effects[4]->StartTimer(0.5f);//set duration of effect

			//while kirby was hit while inhaling
			if (kirby->GetState() == inhaling) {
				effects[0]->StartTimer(0.1f);//remove enemy inhaling effect
				effects[1]->StartTimer(0.1f);//remove enemy pulling effect
			}

			kirby->SetState(hitEnemy);
			kirby->SetHitEnemy();
			continue;
		}
		
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
			effects[3]->SetKirbyStarExplodeOnEnemy(enemies[j]->GetPosition());
			effects[3]->StartTimer(0.2f);

			enemies[j]->SetDeathStart();//start enemy death timer
		}
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
		effects[1]->StartTimer(0.1f);//remove enemy pulling effect
	}
	kirby->SetState(hitEnemy);
	kirby->SetHitEnemy();
}

void KirbyGame::CheckAbility()
{
	if (enemySwallowed.size() == 0) {
		return;
	}
	int abi = -1;
	for (pair<Enemy*, int> enemy : enemySwallowed) {
		string ability = enemy.first->GetAbility();
		int cmp1 = ability.compare("spark");
		int cmp2 = ability.compare("beam");
		
		//enemy has ability
		if (cmp1 == 0 && cmp2 == 0) {//random if both swallowed
			abi = rand() % 2;
		}
		else if (cmp1 == 0) {
			abi = 0;
		}
		else if (cmp2 == 0) {
			abi = 1;
		}
	}
	if (abi == 0) {
		kirby->SetAbility(0);
	}
	else if (abi == 1) {
		kirby->SetAbility(1);
	}
	else {
		kirby->SetAbility(Ability::none);
	}
}
