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
		swallowing
	};
}


class KirbyEffect {
public:
	KirbyEffect();
	~KirbyEffect();
	void SetKirbyPos(Vector3 kirbyPos, bool left); 
	void LoadTextureList();
	void SetKirbyEat();
	void SetKirbySwallow(vector<class Enemy*>& enemySwallowed);
	void SetKirbyBlowStar();//kirby default attack with swallowed enemy
	void UpdateEatEffect();
	bool UpdateSwallowEffect();
	bool UpdateBlowEffect(float UpdateBlowEffect);//return true if enemy has been hit
	bool isTimerSet() { return setTimer; };
	void StartTimer(float duration);
	bool EndTimer();
	void UpdateEffect(float deltaTime);
	void RenderEffect();
	void StopEffect();
	void Update(float deltaTime);
	class Rect* GetRect() { return rectEffect0; };
	
	int GetCurrentEffect() { return currentEffect; };
	vector<class AnimationRect*> GetAnimationRects() { return animations; }

private:
	int pngSplit[15] = {
		1,4,1,3,1,5,2,1,4,4,3,8,2,2,2
	};
	wstring pngList[15] = {
		L"kirbybeam.png",//1
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
		L"kirbywaterblowup.png"//2
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
	vector<pair<class Enemy*, int>> enemySwallow;//int for current animation idx of enemy to fix
	Vector3 effectStartPos = Values::ZeroVec3;
	bool effectLeft = true;
	float time = 0.0f;
	bool setTimer = false;
	float duration = 0.0f;
	class Rect* rectEffect0 = nullptr;
};