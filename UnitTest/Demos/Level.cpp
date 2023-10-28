#include "stdafx.h"
#include "Geomatries/TextureRect.h"
#include "Geomatries/Rect.h"
#include "Level.h"
#include "ReadCoordinate.h"
#include "Enemy.h"
#include "EnemyInfo.h"
#include "Boss.h"

Level::Level(Vector3 pos, wstring pngName)
{
	this->position = pos;
	size = Vector3(WinMaxHeight * 6 + 60, WinMaxHeight, 0);
	levelMap = new TextureRect(position, size, 0.0f,
		TexturePath + L"backGround/" + pngName + L".png");
	FileReader fr;
	wstring txtName = pngName + L".txt";
	vector<Vector3> tmp = fr.ReadFile(CoordPath + String::ToString(txtName));
	for (int i = 0; i < tmp.size() / 2; i++) {
		Vector3 pos = tmp[2 * i];
		Vector3 size = tmp[2 * i + 1];
		float angle_in_degrees = (float)pos.z;
		pos.z = 0.0f;
		size.z = 0.0f;
		//cout << pos.z << "\n";
		float angle_in_radians = angle_in_degrees * (3.141592 / 180.0f);
		rects.push_back(new Rect(pos, size, angle_in_radians));
	}

	slopeRange.push_back({ 10092.0f , 10232.0f });//up 45degree
	slopeRange.push_back({ 10232.0f , 10502.0f });//up 27degree
	slopeRange.push_back({ 10590.0f , 10854.0f });//down 27degree
	slopeRange.push_back({ 10854.0f , 10994.0f });//down 45degree
	slopeRange.push_back({ 11468.0f , 11746.0f });//up 27degree
	slopeRange.push_back({ 12384.0f , 12790.0f });//down 27degree

	enemyInfo = new EnemyInfo();
	//if stage 3 make additional wall that limits enemy movement
	if (pngName.compare(L"world1stage3") == 0) {
		limitEnemyMove.push_back(new Rect({ 16170.0f, 600.0f, 0.0f },
			{ 60.0f, 600.0f, 0.0f }, 0.0f));
		limitEnemyMove.push_back(new Rect({ 16900.0f, 600.0f, 0.0f }, 
			{ 50.0f, 600.0f, 0.0f}, 0.0f));
		limitEnemyMove.push_back(new Rect({ 17290.0f, 600.0f, 0.0f },
			{ 60.0f, 600.0f, 0.0f }, 0.0f));
		limitEnemyMove.push_back(new Rect({ 17730.0f, 550.0f, 0.0f },
			{ 60.0f, 550.0f, 0.0f }, 0.0f));
	}
}

//this constructor is for boss level
Level::Level(Vector3 pos, Vector3 size, wstring pngName)
{
	this->position = pos;
	levelMap = new TextureRect(position, size, 0.0f,
		TexturePath + L"backGround/" + pngName + L".png");
	FileReader fr;
	wstring txtName = pngName + L".txt";
	vector<Vector3> tmp = fr.ReadFile(CoordPath + String::ToString(txtName));
	for (int i = 0; i < tmp.size() / 2; i++) {
		Vector3 pos = tmp[2 * i];
		Vector3 size = tmp[2 * i + 1];
		float angle_in_degrees = (float)pos.z;
		pos.z = 0.0f;
		size.z = 0.0f;
		//cout << pos.z << "\n";
		float angle_in_radians = angle_in_degrees * (3.141592 / 180.0f);
		rects.push_back(new Rect(pos, size, angle_in_radians));
	}
	enemyInfo = new EnemyInfo();

	boss = new TreeBoss(Vector3(21491.0f, -95.0f, 0.0f),
		Vector3(290.0f, 430.0f, 0.0f));
}

Level::~Level()
{
	vector<pair<float, float>>().swap(slopeRange);
	SAFE_DELETE(enemyInfo);
	vector<Enemy*>().swap(enemies);
	SAFE_DELETE(levelMap);
	vector<Rect*>().swap(rects);
	SAFE_DELETE(boss);
}

void Level::Init()
{
}

void Level::Destroy()
{
}

void Level::Update()
{
	//only apply for boss
	if (boss) {
		int bossPrevState = boss->GetPrevState();
		int bossState = boss->GetState();
		//atk => apple atk
		if (bossPrevState == 1 && bossState == 2) {
			CreateApples();
		}
		//clear apple
		if (bossPrevState == 2 && bossState == 0) {
			vector<Enemy*>().swap(enemies);
		}

		boss->Update();
	}
	//check enemy death
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->GetState() == 3) {//if enemy in death state
			enemyDeathCnt++;
			deathEffects.push_back(new KirbyEffect());
			deathEffects.back()->SetEnemyDeathEffect(enemies[i]->GetPosition());
			deathEffects.back()->StartTimer(0.8f);

			//change position of enemy to grave
			enemies[i]->SetPosition(grave);
			//erase enemy from vector
			enemies.erase(enemies.begin() + i);

			i--;
		}
	}

	levelMap->Update();
	for (int i = 0; i < rects.size(); i++) {
		rects[i]->Update();
	}
	for (size_t i = 0; i < limitEnemyMove.size(); i++)
	{
		limitEnemyMove[i]->Update();
	}

	EnemyCollisions();
	//remove death effects
	for (size_t i = 0; i < deathEffects.size(); i++)
	{
		if (!deathEffects[i]->isTimerSet()) {
			deathEffects.erase(deathEffects.begin() + i);
			i--;
		}
		else {
			//else update
			deathEffects[i]->UpdateDeathEffect(Time::Delta());
		}
	}
}

void Level::Render()
{
	levelMap->Render();
	//update all the enemies
	for (class Enemy* enemy : enemies) {
		enemy->Render();
	}
	for (int i = 0; i < rects.size(); i++) {
		rects[i]->Render();
	}
	for (size_t i = 0; i < limitEnemyMove.size(); i++)
	{
		limitEnemyMove[i]->Render();
	}
	for (size_t i = 0; i < deathEffects.size(); i++)
	{
		deathEffects[i]->RenderDeathEffect();
	}

	if (boss) {
		boss->Render();
	}
}

void Level::PostRender()
{
}

void Level::GUI()
{
}

Vector3 Level::GetLT()
{
	return levelMap->GetLT();
}

Vector3 Level::GetRB()
{
	return levelMap->GetRB();
}

void Level::SetKirbyPosition(Vector3 kirbyPosition)
{
	this->kirbyPosition = kirbyPosition;
}

void Level::EnemyCollisions()
{
	//enemies in level
	for (size_t i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->GetState() != 3) {
			//set enemy collision to default
			enemies[i]->SetHitGround(false);
			enemies[i]->SetHitLeft(false);
			enemies[i]->SetHitRight(false);
			//set kirby position for enemy to chase
			enemies[i]->SetKirbyPos(kirbyPosition);

			Rect* enemyRect = enemies[i]->GetRect();
			//collision of enemy and level
			EnemyCollisionLevel(enemyRect, i);
			enemies[i]->SetKirbyPos(kirbyPosition);

			//if (CheckEnemyInRange(enemies, i)) {
			enemies[i]->Update();
			//}
		}
	}
}

void Level::EnemyCollisionLevel(Rect* enemyRect, int enemyIdx)
{
	BoundingBox* enemyBox = nullptr;
	if (enemyRect) {
		enemyBox = enemyRect->GetBox();
	}
	//limit enemy movement
	for (size_t i = 0; i < limitEnemyMove.size(); i++)
	{
		BoundingBox* limitBox = limitEnemyMove[i]->GetBox();
		if (enemyBox && BoundingBox::OBB(enemyBox, limitBox)) {
			FixEnemyPosition(limitEnemyMove[i], enemyIdx);
		}
	}
	//iterate level rects
	for (int i = 0; i < rects.size(); i++) {
		if (enemyBox && BoundingBox::OBB(enemyBox, rects[i]->GetBox())) {
			float rotation = rects[i]->GetRotation();
			Vector3 enemyPos = enemies[enemyIdx]->GetPosition();
			//collision with slope
			if (rotation > 0.1f) {//upper
				int idx = CheckSlopeRange(enemyPos.x);
				if (idx > -1) {
					SetEnemyPosForSlope(idx, enemyPos, rotation, enemies[enemyIdx]);
				}
			}
			else if (rotation < -0.1f) {//down
				int idx = CheckSlopeRange(enemyPos.x);
				if (idx > -1) {
					SetEnemyPosForSlope(idx, enemyPos, rotation, enemies[enemyIdx]);
				}
			}
			else {
				FixEnemyPosition(rects[i], enemyIdx);
			}
		}
	}
}

void Level::IntersectRect(Rect* enemyRect, Rect* levelRect, pair<Vector3, Vector3>& intersection) {
	Vector3 enemyLT = enemyRect->GetLT();
	Vector3 worldLT = levelRect->GetLT();
	Vector3 enemyRB = enemyRect->GetRB();
	Vector3 worldRB = levelRect->GetRB();


	float left = max(enemyLT.x, worldLT.x);
	float top = min(enemyLT.y, worldLT.y);
	float right = min(enemyRB.x, worldRB.x);
	float bottom = max(enemyRB.y, worldRB.y);
	if (left >= right || top <= bottom) {
		intersection = make_pair(Values::ZeroVec3, Values::ZeroVec3);
		return;
	}
	intersection = make_pair(Vector3(left, top, 0), Vector3(right, bottom, 0));
	return;
}

void Level::FixEnemyPosition(Rect* levelRect, int idx)
{
	pair<Vector3, Vector3> intersection;
	IntersectRect(enemies[idx]->GetRect(), levelRect, intersection);

	Vector3 enemyPos = enemies[idx]->GetPosition();
	Vector3 enemySize = enemies[idx]->GetRect()->GetSize();

	Vector3 enemyLT = enemies[idx]->GetLT();
	Vector3 enemyRB = enemies[idx]->GetRB();

	Vector3 worldRectPos = levelRect->GetPosition();
	Vector3 worldLT = levelRect->GetLT();
	Vector3 worldRB = levelRect->GetRB();

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

vector<class Enemy*> Level::GetEnemies()
{
	return enemies;
}

int Level::CheckSlopeRange(float positionX)
{
	for (int i = 0; i < slopeRange.size(); i++) {
		if (slopeRange[i].first <= positionX && positionX <= slopeRange[i].second) {
			return i;
		}
	}

	return -1;
}

void Level::SetEnemyPosForSlope(int idx, Vector3 enemyPos, float rotation, class Enemy* enemy)
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

bool Level::CheckEnemyInRange(vector<Enemy*> enemies, int enemyIdx)
{
	//measure x distance between kirby and enemy
	float distanceX = abs(kirbyPosition.x - enemies[enemyIdx]->GetPosition().x);
	if (distanceX < WinMaxWidth / 2 + 200.0f) {
		return true;
	}

	return false;
}

void Level::CreateRandomEnemies()
{
	for (int i = 0; i < 20; i++) {
		choice = rand() % 3;
		xDistance = rand() % 100 + 150;
		enemyName = enemyNames[choice];

		enemies.push_back(new Enemy({ position.x - 1000.0f + xDistance * i, position.y + 200.0f, 0 }, { 128.0f, 128.0f, 0.0f }, enemyName, enemyInfo));
	}
}

void Level::CreateApples()
{
	enemyName = "apple";

	appleX[0] = rand() % 500 + 1;
	appleX[1] = rand() % 500 + 1;
	appleX[2] = rand() % 500 + 1;

	while (abs(appleX[0] - appleX[1]) < 150 ||
		abs(appleX[0] - appleX[2]) < 150 ||
		abs(appleX[1] - appleX[2]) < 150)
	{
		appleX[0] = rand() % 500 + 1;
		appleX[1] = rand() % 500 + 1;
		appleX[2] = rand() % 500 + 1;
	}

	for (size_t i = 0; i < 3; i++)
	{
		appleX[i] = position.x - appleX[i];
		enemies.push_back(new Enemy({ appleX[i], position.y - 280.0f, 0 }, 
			{ 64.0f, 64.0f, 0.0f }, enemyName, enemyInfo));

		if (kirbyPosition.x < appleX[i]) {
			enemies[i]->SetLeft(true);
		}
		else {
			enemies[i]->SetLeft(false);
		}
	}
	int order[3] = { 0, 1, 2 };
	//shuffle
	for (int i = 0; i < 20; i++) {
		int tmp1 = rand() % 3;
		int tmp2 = rand() % 3;
		int tmp = order[tmp1];
		order[tmp1] = order[tmp2];
		order[tmp2] = tmp;
	}
	//set order
	for (size_t i = 0; i < 3; i++)
	{
		enemies[order[i]]->WaitApple(2.0f * i);
	}
}

Rect* Level::GetBossRect()
{
	return boss->GetRect();
}
