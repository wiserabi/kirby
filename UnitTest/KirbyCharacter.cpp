#include "stdafx.h"
#include "KirbyCharacter.h"
#include "Utilities/Animator.h"
#include "UI/HUD.h"

KirbyCharacter::KirbyCharacter(Vector3 position, Vector3 size)
	:AnimationRect(position, size, false)
{
	Animator* tempAnimator = new Animator();

	{
		//Texture2D* srcTex = new Texture2D(TexturePath + L"player.png");
		//AnimationClip* RunR = new AnimationClip(L"RunR", srcTex, 10,
		//	Values::ZeroVec2,
		//	Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 0.5f));
		//
		//AnimationClip* RunL = new AnimationClip(L"RunL", srcTex, 10,
		//	Vector2(0, srcTex->GetHeight() * 0.5f),
		//	Vector2(srcTex->GetWidth(), srcTex->GetHeight()));

		Texture2D* srcTex = new Texture2D(TexturePath + L"kirbyAnim/kirbywalkright.png");
		AnimationClip* RunR = new AnimationClip(L"WalkR", srcTex, 4,
			Vector2(0, srcTex->GetHeight() * 0.0f),
			Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 1.0f));
		AnimationClip* RunL = new AnimationClip(L"WalkL", srcTex, 4,
			Values::ZeroVec2,
			Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 1.0f));

		tempAnimator->AddAnimClip(RunR);
		tempAnimator->AddAnimClip(RunL);

		tempAnimator->SetCurrentAnimClip(L"WalkR");
		SetAnimator(tempAnimator);
		SAFE_DELETE(srcTex);
	}
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

	if (key->Press(VK_RIGHT)) {
		__super::SetDirection(Vector3(1, 0, 0));
		__super::SetVelocity(100 * delta);
		current = L"WalkR";
		__super::SetLeft(false);
		
	}
	else if (key->Press(VK_LEFT)) {
		__super::SetDirection(Vector3(-1, 0, 0));
		__super::SetVelocity(100 * delta);
		current = L"WalkL";
		__super::SetLeft(true);
	}
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
