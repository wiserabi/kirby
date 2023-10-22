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

private:
	Vector3 position;
	Vector3 size;
	class TextureRect* levelMap;
	//class Rect* guiRect = nullptr;

};