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
	dash,
	endDash,
	gethit,
	attacking,
	flying,
	flyup,
	falldown,
	inhaled,
	exhale, 
	exhaling,
	swallowed,
	bounce,
	flatten,
	jump,
	jumpdown,
	jumpmin,
	headdown,
	exhaled,
	drift,
	sandwiched
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
	virtual void SetPosition(Vector3 pos);

	bool Move1(float delta, class Keyboard* key);
	bool Move2(float delta, class Keyboard* key);
	bool Move3(float delta, class Keyboard* key);

	bool Inhale(float delta, class Keyboard* key);//for eating enemy
	bool Inhaled(float delta, class Keyboard* key);
	bool Exhale(float delta, class Keyboard* key);
	bool Exhaled(float delta, class Keyboard* key);
	bool Exhaling(float delta, class Keyboard* key);
	bool Jump(float delta, class Keyboard* key);
	bool Walk(float delta, class Keyboard* key);
	bool Run(float delta, class Keyboard* key);
	bool HeadDown(float delta, class Keyboard* key);
	bool Slide(float delta, class Keyboard* key);
	bool WalkingToIdle(float delta, class Keyboard* key);
	bool Flatten(float delta, class Keyboard* key);
	bool Bounce(float delta, class Keyboard* key);
	bool SquashDown(float delta, class Keyboard* key);
	bool FloatUp(float delta, class Keyboard* key);
	bool FlyUp(float delta, class Keyboard* key);
	bool StartFly(float delta, class Keyboard* key);
	bool FallDown(float delta, class Keyboard* key);
	bool JumpDown(float delta, class Keyboard* key);
	bool JumpMin(float delta, class Keyboard* key);
	bool JumpLong(float delta, class Keyboard* key);
	bool JumpEnd(float delta, class Keyboard* key);
	bool JumpStart(float delta, class Keyboard* key);
	bool JumpUp(float delta, class Keyboard* key);
	bool Idle(float delta, class Keyboard* key);
	bool Dash(float delta, class Keyboard* key);
	bool EndDash(float delta, class Keyboard* key);
	bool Drift(float delta, class Keyboard* key);
	bool SandWiched(float delta, class Keyboard* key);
	
	void Swallow();
	void Attack();
	void ApplyGravity();
	void SetHitGround(bool hitGround) { this->hitGround = hitGround; }
	void SetHitLeft(bool hitLeft) { this->hitLeft = hitLeft; }
	void SetHitRight(bool hitRight) { this->hitRight = hitRight; }
	void ChangeAnimation(wstring clipName, float speed, Vector3 dir, uint currentFrame, bool setFrame);
	void ChangeBoundingBox();

	Vector3 GetPosition();
	boolean isKirbyInWorld() { return kirbyInWorld; }
	class Rect* GetRect();

private:
	int health;
	int lifeCount;
	Ability ability;
	State state = falldown;

	ID3D11SamplerState* point[2];
	ID3D11BlendState* bpoint[2];
	wstring current = L"WalkR";
	boolean kirbyInWorld = true;
	class Rect* rect = nullptr;
	vector<class Rect*> list;
	bool hitGround = false;
	bool hitLeft = false, hitRight = false;
	float startFalling = 0.0f;
	float startSqueeze = 0.0f;
	float startBounce = 0.0f;
	float startJump = 0.0f;
	float startInhaled = 0.0f;
	float delayAfterExhale = 0.0f;
	Vector3 dir = Values::ZeroVec3;
	bool prevRight = false, prevLeft = false;
	float prevRightTime = 0.0f, prevLeftTime = 0.0f;
	float endDashTime = 0.0f;
	float startSandwich = 0.0f;
	float prevkirbyX;
};

