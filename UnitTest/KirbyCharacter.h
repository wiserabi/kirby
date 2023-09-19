#pragma once

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
	inhale,
	exhale,
	slide,
	swallowed
};

class KirbyCharacter : IObject
{
public:
	KirbyCharacter();
	~KirbyCharacter();

	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUI() override;
	void Move();
	void Inhale();
	void Exhale();
	void Swallow();
	void Attack();

private:
	Vector2 position;
	Vector2 direction;
	float velocity;
	int health;
	int lifeCount;
	Ability ability;
	State state;

	class AnimationRect* kirbyAnimation = nullptr;
};

