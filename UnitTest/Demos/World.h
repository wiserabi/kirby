#pragma once

#define LEVELNUM 4

class World : IObject
{
public:
	World();
	~World();
	//Enemy
	//Items
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUI() override;
	Vector3 GetLT();
	Vector3 GetRB();

	Vector3 getWorldSize() { return worldSize; }
	Vector3 getWorldPosition() { return worldPos; }
	vector<class Rect*> GetRects(){ return rects; }
	void SetColor(int index, Color color);
	vector<class Level*> GetLevels() { return levels; }
	void SetKirbyPos(Vector3 kirbyPos);
	void SetKirbyLocation(int kirbyLocation) { this->kirbyLocation = kirbyLocation; };

private:
	Vector3 worldSize;
	Vector3 worldPos;
	class TextureRect* worldMap = nullptr;
	class Rect* guiRect = nullptr;
	vector<class Rect*> rects;
	vector<class Level*> levels;//stages
	Vector3 kirbyPos;
	bool enableGui = false;
	int kirbyLocation = 0;

	wstring levelPng[LEVELNUM] = {
		L"world1stage1",
		L"world1stage2",
		L"world1stage3",
		L"boss1stage",
	};
};