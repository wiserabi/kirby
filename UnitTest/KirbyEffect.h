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
		waterblowup
	};
}


class KirbyEffect {
public:
	KirbyEffect();
	~KirbyEffect();
	void SetKirbyPos(Vector3 kirbyPos, bool left); 
	void LoadTextureList();
	void SetKirbyEat();
	void UpdateEatEffect(float deltaTime);
	void UpdateEffect(float deltaTime);
	void RenderEffect();
	void StopEffect();
	void Update(float deltaTime);
	vector<class AnimationRect*> GetTexture() { return animations; }

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
};