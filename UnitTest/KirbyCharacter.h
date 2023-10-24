#pragma once

#define PNGNUM 16
#define DOORNUM 5

enum Location {
	WORLD,
	LEVEL1,
	LEVEL2,
	LEVEL3,
};
enum Ability
{
	spark,
	beam,
	fire,
	fireball,
	crash,
	sword,
	freeze,
	cutter,
	none
};

enum State {
	idle,
	walking,
	inhale,
	dash,
	endDash,
	gethit,
	attacking,
	flying,
	flyup,
	falldown,
	inhaled,
	inhaling,
	exhale,
	exhaling,
	bounce,
	flatten,
	jump,
	jumpdown,
	jumpmin,
	headdown,
	exhaled,
	drift,
	sandwiched,
	stopInhaling,
	swallowing,
	eatidle,
	eatandwalk,
	hitEnemy,
	opendoor,
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
	bool Inhaling(float delta, class Keyboard* key);

	bool Exhale(float delta, class Keyboard* key);
	bool Exhaled(float delta, class Keyboard* key);
	bool Exhaling(float delta, class Keyboard* key);
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
	bool StopInhaling(float delta, class Keyboard* key);
	bool Swallowing(float delta, class Keyboard* key);
	bool EatIdle(float delta, class Keyboard* key);
	bool EatAndWalk(float delta, class Keyboard* key);
	bool Attack(float delta, class Keyboard* key);
	bool HitEnemy(float delta, class Keyboard* key);
	bool UseAbility(float delta, class Keyboard* key);
	bool RemoveAbility(float delta, class Keyboard* key);
	bool OpenDoorStart(float delta, class Keyboard* key);
	bool OpenDoor(float delta, class Keyboard* key);
	int CheckOpenDoor();
	void Teleportation(int doorIdx);

	void SetHitEnemy();
	float GetHitEnemy();
	
	void SetHitGround(bool hitGround) { this->hitGround = hitGround; }
	void SetHitLeft(bool hitLeft) { this->hitLeft = hitLeft; }
	void SetHitRight(bool hitRight) { this->hitRight = hitRight; }
	void ChangeBoundingBox();
	State GetState();
	State GetPrevState();

	void SetState(State state) { this->state = state; }

	void SetAttackDelay();
	Vector3 GetPosition();
	int getKirbyLocation() { return kirbyInWorld; }
	class Rect* GetRect();
	void SetEffectHit(int effectHitDir) { this->effectHitDir = effectHitDir; }
	void SetAbility(int ability);
	void StartUseAbility(class Keyboard* key);
	class KirbyEffect* GetSparkEffect() { return sparkEffect; }
	class KirbyEffect* GetBeamEffect() { return beamEffect; }
	Vector3 GetDirection();//get direction vector of kirby

private:
	int health;
	int lifeCount;
	Ability ability;
	bool abilitySet = false;
	State state = falldown;
	State prevState = falldown;
	ID3D11SamplerState* point[2];
	ID3D11BlendState* bpoint[2];

	wstring current = L"WalkR";
	Location kirbyInWorld = WORLD;
	class Rect* rect = nullptr;
	vector<class Rect*> list;
	bool hitGround = false;
	bool hitLeft = false;
	bool hitRight = false;
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
	float prevkirbyX = 0.0f;
	float startInhale = 0.0f;
	float stopInhale = 0.0f;
	float startEatWalk = 0.0f;
	float attackDelay = 0.0f;
	float hitEnemyTime = 0.0f;//moment when kirby hit enemy
	float openDoorTime = 0.0f;
	int effectHitDir = -1;
	bool abilityUse = false;
	class KirbyEffect* sparkEffect;
	class KirbyEffect* beamEffect;
	class KirbyEffect* getAbilityEffect;
	class KirbyEffect* removeAbilityEffect;
	int doorIdx = -1;


	wstring animationPng[PNGNUM] = {
		L"kirbywalkright.png",
		L"kirbyidle.png",
		L"kirbyup.png",
		L"kirbyfly.png",
		L"kirbydown.png",
		L"kirbyjump.png",
		L"kirbyslidedown.png",
		L"kirbySandwiched.png",
		L"kirbyinhale.png",
		L"kirbyeatidle.png",
		L"kirbyeatandwalk.png",
		L"kirbyattack.png",
		L"kirbyouch.png",
		L"kirbyspark.png",
		L"kirbybeam.png",
		L"kirbydoor.png",
	};
	wstring motions[PNGNUM] = {
		L"WalkR",
		L"Idle",
		L"flyUp",
		L"inhaled",
		L"exhaling",
		L"jump",
		L"slide",
		L"sandwiched",
		L"inhale",
		L"eatidle",
		L"eatandwalk",
		L"attack",
		L"ouch",
		L"spark",
		L"beam",
		L"door",
	};
	int split[PNGNUM] = {
		4,2,4,2,4,6,2,1,2,1,4,3,1,2,2,2
	};

	vector<class Rect*> doors;
	Vector3 doorPos[DOORNUM] = {
		Vector3(386.0f, 244.0f, 0.0f),//stage1
		Vector3(968.0f, 64.0f, 0.0f),//stage2
		Vector3(8058.0f, 426.0f, 0.0f),//level1
		Vector3(13098.0f, 358.0f, 0.0f),//level2
		Vector3(18062.0f, 352.0f, 0.0f),//level3
	};
	Vector3 doorSize[DOORNUM] = {
		Vector3(64.0f, 90.0f, 0.0f),
		Vector3(64.0f, 90.0f, 0.0f),
		Vector3(68.0f, 98.0f, 0.0f),
		Vector3(68.0f, 98.0f, 0.0f),
		Vector3(56.0f, 88.0f, 0.0f),
	};
};

