#pragma once

namespace Effect {
	enum EffectName {
		beam,
		bigstars,
		blow,
		bomb,
		bubblewater,
		crash,
		dash,
		eat,
		fire,
		freeze,
		explode,
		twostar,
		waterblowdown,
		waterblowright,
		waterblowup,
		spark,
		swallowing,
	};
}


class KirbyEffect {
public:
	KirbyEffect();
	~KirbyEffect();
	void SetKirbyPos(Vector3 kirbyPos, bool left); 
	void LoadTextureList();
	void SetKirbyEat();
	void SetKirbySwallow(vector<pair<class Enemy*, int>>& enemySwallowed);
	void SetKirbyBlowStar();//kirby default attack with swallowed enemy
	void SetKirbyStarExplodeOnEnemy(Vector3 Pos);//when star hits enemy
	void SetEnemyDeathEffect(Vector3 pos);//when enemy gets killed effect
	void SetHitEffect();//when kirby is hit by enemy
	void SetKirbyBlowAir();//kirby blow effect
	void SetSparkEffect(Vector3 pos);
	void SetBeamEffect(Vector3 pos, bool leftSide);

	void UpdateEatEffect();
	bool UpdateSwallowEffect(vector<pair<class Enemy*, int>>& enemySwallow);
	bool UpdateBlowEffect(float UpdateBlowEffect);//return true if enemy has been hit
	void UpdateExplodeOnEnemy();
	void UpdateDeathEffect(float delta);
	void UpdateHitEffect(float delta);
	void UpdateBlowAir(float delta);
	void UpdateSparkEffect(float delta);
	void UpdateBeamEffect(float delta, float leftSide);
	

	bool isTimerSet() { return setTimer; };
	void StartTimer(float duration);
	void UpdateEffect(float deltaTime);
	void RenderSwallowEffect(vector<pair<class Enemy*, int>>& enemySwallow);
	void RenderDeathEffect();
	void RenderHitEffect();
	void RenderBlowAir();
	void RenderSparkEffect();
	void RenderBeamEffect();

	void RenderEffect();
	void StopEffect();
	class Rect* GetRect() { return rectEffect0; };
	
	int GetCurrentEffect() { return currentEffect; };
	vector<class AnimationRect*> GetAnimationRects() { return animations; }

private:
	int pngSplit[16] = {
		11,4,1,3,1,5,2,1,4,4,3,8,2,2,2,8
	};
	wstring pngList[16] = {
		L"kirbybeam.png",//11
		L"kirbybigstars.png",//4
		L"kirbybloweffect.png",//1
		L"kirbybomb.png",//3
		L"kirbybubblewater.png",//1
		L"kirbycrasheffect.png",//5
		L"kirbydasheffect.png",//2
		L"kirbyeateffect.png",//1
		L"kirbyfireeffect.png",//4
		L"kirbyfreezeeffect.png",//4
		L"kirbyexplodeeffect.png",//3
		L"kirbytwostar.png",//8
		L"kirbywaterblowdown.png",//2
		L"kirbywaterblowright.png",//2
		L"kirbywaterblowup.png",//2
		L"kirbysparkeffect.png"//8
	};
	vector<class AnimationRect*> animations;
	vector<class Animator*> animatorList;
	bool collidable = false;
	bool useKirbyPos = false;
	Vector3 kirbyPos = Values::ZeroVec3;
	int currentEffect = 0;
	class BezierCurves* curves = nullptr;
	int count = 0;
	vector<Vector2> controlPoints;
	bool left = true;
	Vector3 effectStartPos = Values::ZeroVec3;
	bool effectLeft = true;
	float time = 0.0f;
	bool setTimer = false;
	float duration = 0.0f;
	class Rect* rectEffect0 = nullptr;
	vector<Vector3> effectStartPositions;
	Vector3 randVec = Values::ZeroVec3;
};