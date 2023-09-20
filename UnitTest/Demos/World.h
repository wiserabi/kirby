#pragma once

class World : IObject
{
public:
	int currentLevel = 1;
	World();
	//Enemy
	//Items
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUI() override;
	Vector3 getWorldSize() { return worldSize; }
	Vector3 getWorldTranslation() { return worldMove; }

private:
	Vector3 worldSize;
	Vector3 worldMove;
	class TextureRect* worldMap;
};