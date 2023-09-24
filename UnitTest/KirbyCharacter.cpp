#include "stdafx.h"
#include "Geomatries/Rect.h"
#include "Geomatries/AnimationRect.h"
#include "KirbyCharacter.h"
#include "Utilities/Animator.h"
#include "UI/HUD.h"
#define VELOCITY 200

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

		Texture2D* srcTex3 = new Texture2D(TexturePath + L"kirbyAnim/kirbyfly.png");
		AnimationClip* inhaled = new AnimationClip(L"inhaled", srcTex3, 2,
			Values::ZeroVec2,
			Vector2(srcTex3->GetWidth(), srcTex3->GetHeight() * 1.0f));

		tempAnimator->AddAnimClip(WalkR);
		tempAnimator->AddAnimClip(WalkL);
		tempAnimator->AddAnimClip(Idle);
		tempAnimator->AddAnimClip(flyUp);
		tempAnimator->AddAnimClip(inhaled);

		tempAnimator->SetCurrentAnimClip(L"WalkR");
		SetAnimator(tempAnimator);
		SAFE_DELETE(srcTex2);
		SAFE_DELETE(srcTex1);
		SAFE_DELETE(srcTex);
	}
	rect = new Rect(position, size / 2, 0.0f);
	rect->SetColor(Color(0.5, 0.5, 0.5, 0.7));
}

KirbyCharacter::~KirbyCharacter()
{
	SAFE_DELETE(rect);
}

void KirbyCharacter::Update()
{
	rect->SetPosition(position);
	rect->Update();
	__super::Update();
}

void KirbyCharacter::Render()
{
	rect->Render();
	__super::Render();
}

void KirbyCharacter::Move()
{
	auto key = Keyboard::Get();
	float delta = Time::Delta();

	Vector3 dir = Values::ZeroVec3;

	if (key->Press(VK_RIGHT)) {
		dir += Values::RightVec;
		__super::SetVelocity(VELOCITY * delta);
		current = L"WalkR";
		__super::SetLeft(false);
		state = walking;
		
	}
	else if (key->Press(VK_LEFT)) {
		dir += Values::LeftVec;
		__super::SetVelocity(VELOCITY * delta);
		current = L"WalkL";
		__super::SetLeft(true);
		state = walking;
	}
	if (state == inhaled && key->Press(VK_UP)) {
		current = L"inhaled";
		dir += Values::UpVec;
		__super::SetVelocity(VELOCITY * delta);
	}
	else if (state == inhaled) {
		current = L"inhaled";
		dir += Values::DownVec;
		__super::SetVelocity(VELOCITY * delta);
	}
	else if (key->Press(VK_UP)) {
		dir += Values::UpVec;
		__super::SetVelocity(VELOCITY * delta);
		current = L"flyUp";
		auto curframe = __super::GetAnimator()->GetCurrentFrameIndex();
		if (curframe == 3) {
			state = inhaled;
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

void KirbyCharacter::ApplyGravity()
{
}

Vector3 KirbyCharacter::GetPosition()
{
	return __super::GetPosition();
}

class Rect* KirbyCharacter::GetRect()
{
	return this->rect;
}
