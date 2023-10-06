#include "stdafx.h"
#include "BezierCurves.h"
#include "KirbyEffect.h"
#include "Geomatries/AnimationRect.h"

#include "Utilities/Animator.h"

KirbyEffect::KirbyEffect()
{
	LoadTextureList();
}

KirbyEffect::~KirbyEffect()
{

}

void KirbyEffect::LoadTextureList()
{
	for (size_t i = 0; i < 15; i++)
	{
		{
			Texture2D* srcTex = new Texture2D(TexturePath + L"kirbyEffect/" + pngList[i]);
			AnimationClip* tmpClip = new AnimationClip(pngList[i], srcTex, pngSplit[i],
				Vector2(0, srcTex->GetHeight() * 0.0f),
				Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 1.0f));
			animatorList.push_back(new Animator());
			animatorList[i]->AddAnimClip(tmpClip);
			animatorList[i]->SetCurrentAnimClip(pngList[i]);
			SAFE_DELETE(srcTex);
		}
	}
}

/*
Eat Effect Animation

For each waterdrops on bezeir curve
kirby's location is p1
The waterdrop move from p3 to p1 
When waterdrop reaches p1 it gets removed
Everytime waterdrop starts at p3 running time is saved
Using when p3 started last time next p3 start time is
calculated using random float from 0.1 ~ 0.5 sec
*/
void KirbyEffect::SetKirbyEat()
{

	currentEffect = Effect::eat;
	//create 50 animationRect
	for (int i = 0; i < 50; i++) {
		animations.push_back(new AnimationRect(Values::ZeroVec3, Vector3(96.0f, 96.0f, 0.0f), false));
		animations[i]->SetAnimator(animatorList[currentEffect]);
	}

	curves = new BezierCurves();
	
	curves->Clear();
	//set bezeir curves for eat effect;
	Vector2 p1 = Vector2(kirbyPos.x, kirbyPos.y);
	Vector2 p2 = Vector2(kirbyPos.x + 100, kirbyPos.y + 50);
	Vector2 p3 = Vector2(kirbyPos.x + 150, kirbyPos.y + 100);
	curves->Add(p1, p2, p3);

	Vector2 p4 = Vector2(kirbyPos.x, kirbyPos.y);
	Vector2 p5 = Vector2(kirbyPos.x + 100, kirbyPos.y - 50);
	Vector2 p6 = Vector2(kirbyPos.x + 150, kirbyPos.y - 100);
	curves->Add(p4, p5, p6);
}

void KirbyEffect::UpdateEatEffect(float deltaTime)
{
	curves->Update();
	vector<Line> lines_ = curves->GetLines();
	
	count = 0;
	//set position for animationRects and update
	for (int i = 0; i < lines_.size(); i++) {
		Line& line = lines_[i];
		for (int j = 0; j < line.water_drops.size(); j++) {
			if (count >= 50) {
				//max 50 eat particles 
				return;
			}
			animations[count]->SetPosition(Vector3(line.water_drops[j].x, line.water_drops[j].y, 0.0f));
			animations[count]->Update(animatorList[currentEffect]);
			count++;
		}
	}
}

void KirbyEffect::UpdateEffect(float deltaTime)
{
	if (currentEffect == Effect::eat) {
		UpdateEatEffect(deltaTime);
		return;
	}
}

void KirbyEffect::RenderEffect()
{
	for (int i = 0; i < count; i++) {
		animations[i]->Render(animatorList[currentEffect]);
	}
}

void KirbyEffect::StopEffect()
{
	count = 0;
	for (auto& it : animations) {
		SAFE_DELETE(it);
	}
	//clear the previous effect animations
	animations.clear();
}

void KirbyEffect::Update(float deltaTime)
{
}