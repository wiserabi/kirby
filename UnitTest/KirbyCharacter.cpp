#include "stdafx.h"
#include "KirbyCharacter.h"
#include "Utilities/Animator.h"
#include "UI/HUD.h"

KirbyCharacter::KirbyCharacter(Vector3 position, Vector3 size)
	:AnimationRect(position, size, false)
{
	Animator* tempAnimator = new Animator();

	{
		Texture2D* srcTex = new Texture2D(TexturePath + L"kirbyAnim/kirbywalkright.png");
		AnimationClip* WalkR = new AnimationClip(L"WalkR", srcTex, 4,
			Vector2(0, srcTex->GetHeight() * 0.0f),
			Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 1.0f));
		AnimationClip* WalkL = new AnimationClip(L"WalkL", srcTex, 4,
			Values::ZeroVec2,
			Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 1.0f));

		Texture2D* srcTex1 = new Texture2D(TexturePath + L"kirbyAnim/kirbyidle.png");
		AnimationClip* Idle = new AnimationClip(L"Idle", srcTex1, 2,
			Values::ZeroVec2,
			Vector2(srcTex1->GetWidth(), srcTex1->GetHeight() * 1.0f));

		Texture2D* srcTex2 = new Texture2D(TexturePath + L"kirbyAnim/kirbyup.png");
		AnimationClip* flyUp = new AnimationClip(L"flyUp", srcTex2, 4,
			Values::ZeroVec2,
			Vector2(srcTex2->GetWidth(), srcTex2->GetHeight() * 1.0f));

		tempAnimator->AddAnimClip(WalkR);
		tempAnimator->AddAnimClip(WalkL);
		tempAnimator->AddAnimClip(Idle);
		tempAnimator->AddAnimClip(flyUp);

		tempAnimator->SetCurrentAnimClip(L"WalkR");
		SetAnimator(tempAnimator);
		SAFE_DELETE(srcTex2);
		SAFE_DELETE(srcTex1);
		SAFE_DELETE(srcTex);
	}
	box = new BoundingBox(position, size, 0.0f);
}

KirbyCharacter::~KirbyCharacter()
{
}

void KirbyCharacter::Update()
{
	__super::Update();
}

void KirbyCharacter::Render()
{
	__super::Render();
}

void KirbyCharacter::Move()
{
	auto key = Keyboard::Get();
	float delta = Time::Delta();

	Vector3 dir = Vector3(0, 0, 0);

	if (key->Press(VK_RIGHT)) {
		dir += Vector3(1, 0, 0);
		__super::SetVelocity(100 * delta);
		current = L"WalkR";
		__super::SetLeft(false);
		state = walking;
		
	}
	else if (key->Press(VK_LEFT)) {
		dir += Vector3(-1, 0, 0);
		__super::SetVelocity(100 * delta);
		current = L"WalkL";
		__super::SetLeft(true);
		state = walking;
	}
	if (state == inhale) {

	}
	else if (key->Press(VK_UP)) {
		dir += Vector3(0, 1, 0);
		__super::SetVelocity(100 * delta);
		current = L"flyUp";
		__super::SetLeft(true);
		state = flyup;
		auto curframe = __super::GetAnimator()->GetCurrentFrameIndex();
		if (curframe == 3) {
			state = inhale;
		}
	}

	if(state == idle) 
	{
		__super::SetVelocity(0);
		current = L"Idle";
		state = idle;
	}

	__super::SetDirection(dir);

	__super::GetAnimator()->SetCurrentAnimClip(current);
	__super::Move();
}

void KirbyCharacter::SetAnimator(Animator* animator)
{
	__super::SetAnimator(animator);
}

void KirbyCharacter::Inhale()
{
}

void KirbyCharacter::Exhale()
{
}

void KirbyCharacter::Swallow()
{
}

void KirbyCharacter::Attack()
{
}

Vector3 KirbyCharacter::GetPosition()
{
	return __super::GetPosition();
}
