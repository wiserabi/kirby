#pragma once

class Level : IObject
{
public:
	Level(Vector3 pos, wstring pngName);
	~Level();
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUI() override;
	Vector3 GetLT();
	Vector3 GetRB();
	vector<class Rect*> GetRects() { return rects; }
	void SetKirbyPosition(Vector3 kirbyPosition);
	void EnemyCollisions();
	void EnemyCollisionLevel(Rect* enemyRect, int enemyIdx);
	void IntersectRect(Rect* enemyRect, Rect* levelRect, pair<Vector3, Vector3>& intersection);
	void FixEnemyPosition(Rect* levelRect, int idx);
	vector<class Enemy*> GetEnemies();
	int CheckSlopeRange(float positionX);

	void SetEnemyPosForSlope(int idx, Vector3 enemyPos, float rotation, Enemy* enemy);
	bool CheckEnemyInRange(vector<Enemy*> enemies, int enemyIdx);//update enemy only when it is in range

private:
	Vector3 position;
	Vector3 size;
	class TextureRect* levelMap = nullptr;
	//class Rect* guiRect = nullptr;
	vector<class Rect*> rects;//level collidable objects
	vector<class Enemy*> enemies;
	class EnemyInfo* enemyInfo = nullptr;
	Vector3 kirbyPosition;
	//for slopes in level
	vector<pair<float, float>> slopeRange;
	float floor = 340.0f;
	float floor2 = 476.0f;
	float tan27 = 0.509525449f;
	string enemyNames[3] = {
		"sparky",
		"waddledee",
		"waddledoo"
	};
	int choice = 0;
	float xDistance = 0.0f;
	string enemyName;
	vector<Rect*> limitEnemyMove;
};