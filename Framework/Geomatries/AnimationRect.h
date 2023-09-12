#pragma once

#include "TextureRect.h"

class AnimationRect : public TextureRect
{
public:
	AnimationRect(Vector3 positon, Vector3 size);
	~AnimationRect();

	void Update();
	void Render();

	void Move();

	void SetAnimator(class Animator* animator) { this->animator = animator; }

private:
	class Animator* animator = nullptr;

	ID3D11SamplerState* point[2];
	ID3D11BlendState* bpoint[2];
	wstring prev = L"RunR";
};