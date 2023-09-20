#pragma once
#include "Geomatries/AnimationRect.h"

enum Ability
{
	spark,
	beam,
	fire,
	fireball,
	crash,
	sword,
	freeze,
	cutter
};

enum State {
	idle,
	walking,
	running,
	gethit,
	attacking,
	flying,
	flyup,
	inhale,
	exhale,
	slide,
	swallowed
};

class KirbyCharacter : public AnimationRect
{
public:
	KirbyCharacter(Vector3 position, Vector3 size);
	~KirbyCharacter();

	virtual void Update();
	virtual void Render();

	virtual void Move();

	virtual void SetAnimator(class Animator* animator);

	void Inhale();
	void Exhale();
	void Swallow();
	void Attack();
	boolean isKirbyInWorld() { return kirbyInWorld; }
	Vector3 GetPosition();

private:
	int health;
	int lifeCount;
	Ability ability;
	State state = idle;

	ID3D11SamplerState* point[2];
	ID3D11BlendState* bpoint[2];
	wstring current = L"WalkR";
	boolean kirbyInWorld = true;
};

