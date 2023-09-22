#pragma once

class World : IObject
{
public:
	int currentLevel = 1;
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
	Vector3 getWorldSize() { return worldSize; }
	Vector3 getWorldPosition() { return worldPos; }
	vector<class Rect*> GetRects(){ return rects; }
	void SetColor(int index, Color color);


private:
	Vector3 worldSize;
	Vector3 worldPos;
	class TextureRect* worldMap;
	class Rect* rect;
	vector<class Rect*> rects;
};