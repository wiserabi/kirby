#include "stdafx.h"
#include "Geomatries/Rect.h"
#include "Geomatries/AnimationRect.h"
#include "KirbyCharacter.h"
#include "Utilities/Animator.h"
#include "UI/HUD.h"
#define VELOCITY 200
#define FALLMOTIONCHANGE 1.2f
#define JUMPMIN 0.3
#define JUMPMAX 0.8

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

		Texture2D* srcTex4 = new Texture2D(TexturePath + L"kirbyAnim/kirbydown.png");
		AnimationClip* exhaling = new AnimationClip(L"exhaling", srcTex4, 4,
			Values::ZeroVec2,
			Vector2(srcTex4->GetWidth(), srcTex4->GetHeight() * 1.0f));

		Texture2D* srcTex5 = new Texture2D(TexturePath + L"kirbyAnim/kirbyjump.png");
		AnimationClip* jump = new AnimationClip(L"jump", srcTex5, 6,
			Values::ZeroVec2,
			Vector2(srcTex5->GetWidth(), srcTex5->GetHeight() * 1.0f));
		//kirbyslidedown
		Texture2D* srcTex6 = new Texture2D(TexturePath + L"kirbyAnim/kirbyslidedown.png");
		AnimationClip* slide = new AnimationClip(L"slide", srcTex6, 2,
			Values::ZeroVec2,
			Vector2(srcTex6->GetWidth(), srcTex6->GetHeight() * 1.0f));

		tempAnimator->AddAnimClip(WalkR);
		tempAnimator->AddAnimClip(WalkL);
		tempAnimator->AddAnimClip(Idle);
		tempAnimator->AddAnimClip(flyUp);
		tempAnimator->AddAnimClip(inhaled);
		tempAnimator->AddAnimClip(exhaling);
		tempAnimator->AddAnimClip(jump);
		tempAnimator->AddAnimClip(slide);

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
		__super::SetLeft(false);
		if (state == idle) {
			current = L"WalkR";
			state = walking;
		}
		else if (!hitGround && state != inhaled && state != falldown
			&& state != jump && state != jumpmin && state != jumpdown) {
			state = falldown;
			startFalling = Time::Get()->Running();
		}
	}
	else if (key->Press(VK_LEFT)) {
		dir += Values::LeftVec;
		__super::SetLeft(true);
		if (state == idle) {
			current = L"WalkL";
			state = walking;
		}
		else if (!hitGround && state != inhaled && state != falldown 
			&& state != jump && state != jumpmin && state != jumpdown) {
			state = falldown;
			startFalling = Time::Get()->Running();
		}
	}
	else if (hitGround && state != inhaled && key->Press(VK_DOWN)) {
		dir = Values::ZeroVec3;
		current = L"slide";
		state = slide;
	}
	else if(state == walking && hitGround){
		state = idle;
		current = L"idle";
	}

	if (state == exhaling) {
		current = L"exhaling";
		state == exhaling;
		auto curframe = __super::GetAnimator()->GetCurrentFrameIndex();
		if (curframe == 3) {
			state = falldown;//finish exhale motion
			startFalling = Time::Get()->Running();
			return;
		}
		ChangeAnimation(current, VELOCITY * delta, dir, 0, false);
		return;
	}
	else if (state == flat) {
		dir = Values::ZeroVec3;
		current = L"slide";
		if (Time:: Get()->Running() - startSqueeze > 0.1) {
			state = idle;
		}
		ChangeAnimation(current, VELOCITY * delta, dir, 0, true);
		return;
	}
	else if (state == bounce) {
		float elapsed = Time::Get()->Running() - startBounce;
		dir.y = 0;
		if (elapsed <= 0.3) {
			dir += Values::UpVec;
		}
		else if (elapsed <= 0.6) {
			dir -= Values::UpVec;
		}
		else if (elapsed <= 0.9) {
			dir += Values::DownVec;
		}
		else {
			state = flat;
			startSqueeze = Time::Get()->Running();
		}
		current = L"jump";
		ChangeAnimation(current, VELOCITY * delta, dir, 4, true);
		return;
	}
	//press s when down key pressed
	else if (state == slide && key->Press('S')) {
		//when sliding check if kirby is on the ground
		if (!hitGround) {
			state = falldown;
			startFalling = Time::Get()->Running();
			return;
		}
		dir = Values::ZeroVec3;
		if (__super::GetLeft()) {
			dir += Values::LeftVec;
		}
		else {
			dir += Values::RightVec;
		}
		current = L"slide";
		state = slide;
		ChangeAnimation(current, 2 * VELOCITY * delta, dir, 0, true);
		return;
	}
	else if (state == slide) {
		ChangeAnimation(current, 0, dir, 1, true);
		state = idle;
		return;
	}
	//press s when inhaled
	else if (state == inhaled && key->Press('S')) {
		current = L"exhaling";
		state = exhaling;
		ChangeAnimation(current, VELOCITY* delta, dir, 0, false);
		return;
	}
	else if (state == inhaled && (key->Press(VK_UP) || key->Press('Z'))) {
		current = L"inhaled";
		dir += Values::UpVec;
		__super::GetAnimator()->SetPlayRate(current, 1.0/20.0);
		ChangeAnimation(current, VELOCITY* delta, dir, 0, false);
		return;
	}
	else if (state == flyup) {
		dir += Values::UpVec;
		current = L"flyUp";
		state = flyup;
		auto curframe = __super::GetAnimator()->GetCurrentFrameIndex();
		if (curframe == 3) {
			state = inhaled;
		}
		ChangeAnimation(current, VELOCITY * delta, dir, 0, false);
		return;
	}
	else if (key->Press(VK_UP)) {
		current = L"flyUp";
		state = flyup;
		ChangeAnimation(current, VELOCITY* delta, dir, 0, false);
		return;
	}
	//kirby has inhaled air to float around
	else if (state == inhaled) {
		current = L"inhaled";
		dir += Values::DownVec;
		__super::GetAnimator()->SetPlayRate(current, 1.0 / 10.0);
		ChangeAnimation(current, VELOCITY* delta, dir, 0, false);
		return;
	}
	else if (state == falldown) {
		//when you hit ground while falling
		if (hitGround) {
			//when you fall from higher up bounce once
			if (Time::Get()->Running() - startFalling > FALLMOTIONCHANGE) {
				state = bounce;
				current = L"jump";
				startBounce = Time::Get()->Running();
				return;
			}
			else {
				state = flat;
				current = L"slide";
				startSqueeze = Time::Get()->Running();
				return;
			}
		}
		dir += Values::DownVec;
		current = L"jump";
		if (Time::Get()->Running() - startFalling < FALLMOTIONCHANGE) {
			ChangeAnimation(current, 2 * VELOCITY * delta, dir, 3, true);
			return;
		}
		else {
			ChangeAnimation(current, 2 * VELOCITY* delta, dir, 4, true);
			return;
		}
	}
	else if (state == jumpdown) {
		dir += Values::DownVec;
		current = L"jump";
		if (hitGround) {
			startSqueeze = Time::Get()->Running();
			state = flat;
			return;
		}
		uint curIdx = __super::GetAnimator()->GetCurrentFrameIndex();
		if (curIdx == 4) {
			ChangeAnimation(current, 2 * VELOCITY * delta, dir, 3, true);
			return;
		}
		ChangeAnimation(current, 2 * VELOCITY * delta, dir, 0, false);
		return;
	}
	else if (state == jumpmin) {
		if (Time::Get()->Running() - startJump < JUMPMIN) {
			current = L"jump";
			dir.y = 0;
			dir += Values::UpVec;
			ChangeAnimation(current, 2 * VELOCITY * delta, dir, 5, true);
			return;
		}
		else {
			state = jumpdown;
		}
		return;
	}
	//if player pressed for too long, end the jump
	else if (state == jump && (Time::Get()->Running() - startJump > JUMPMAX)) {
		state = jumpdown;
		return;
	}
	//when user wants to end jump
	else if (key->Up('Z')) {
		//grant minimum jump for short press
		if (state == jump && (Time::Get()->Running() - startJump < JUMPMIN)) {
			state = jumpmin;
		}
		//end the jump if jump time is in range of jumpmin and jumpmax
		else if (state == jump && (Time::Get()->Running() - startJump <= JUMPMAX)) {
			state = jumpdown;
		}
		return;
	}
	//jump when kirby hits the ground
	else if (key->Down('Z') && hitGround) {
		state = jump;
		current = L"jump";
		startJump = Time::Get()->Running();
		return;
	}
	else if (state == jump) {
		current = L"jump";
		dir.y = 0;
		dir += Values::UpVec;
		ChangeAnimation(current, 2 * VELOCITY * delta, dir, 5, true);
		return;
	}
	else if (state == idle)
	{
		current = L"Idle";
		state = idle;
		ChangeAnimation(current, 0, dir, 0, false);
		return;
	}
	ChangeAnimation(current, VELOCITY* delta, dir, 0, false);
}

void KirbyCharacter::ChangeAnimation(wstring clipName, float speed, Vector3 dir, uint currentFrame, bool setFrame)
{
	if (clipName.compare(L"") != 0) {
		__super::GetAnimator()->SetCurrentAnimClip(clipName);
	}
	if (setFrame) {
		__super::GetAnimator()->SetCurrentFrame(currentFrame);
	}
	__super::SetVelocity(speed);
	__super::SetDirection(dir);
	__super::Move();
}

void KirbyCharacter::SetAnimator(Animator* animator)
{
	__super::SetAnimator(animator);
}

void KirbyCharacter::SetPosition(Vector3 pos)
{
	rect->SetPosition(pos);
	__super::SetPosition(pos);
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
