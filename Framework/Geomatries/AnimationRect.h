#pragma once

#include "TextureRect.h"
#include "Utilities/Animator.h"

class AnimationRect : public TextureRect
{
public:
	AnimationRect(Vector3 positon, Vector3 size, boolean left);
	~AnimationRect();

	void Update();
	void Render();
	void Update(class Animator* anim);
	void Render(class Animator* anim);

	void Move();

	void SetAnimator(class Animator* animator) { this->animator = animator; }
	class Animator* GetAnimator() { return animator; }
	void SetLeft(boolean left) { this->left = left; }
	bool GetLeft() { return this->left; }
	
	void SetPosition(Vector3 position) { this->position = position; }
	Vector3 GetPosition() { return position; }

	void SetDirection(Vector3 direction) { this->direction = direction; }
	Vector3 GetSize() { return size; }
	
	void SetVelocity(float velocity) { this->velocity = velocity; }
	void ChangeAnimation(wstring clipName, float speed, Vector3 dir, uint currentFrame, bool setFrame);

private:
	class Animator* animator = nullptr;

	ID3D11SamplerState* point[2];
	ID3D11BlendState* bpoint[2];
	boolean left;

	float velocity = 0.0;
	Vector3 direction = Vector3(0,0,0);
};