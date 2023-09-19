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

private:
	class TextureRect* worldMap;
};