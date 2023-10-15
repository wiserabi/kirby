#include "stdafx.h"
#include "Geomatries/Rect.h"
#include "Geomatries/AnimationRect.h"
#include "KirbyCharacter.h"
#include "Utilities/Animator.h"
#include "UI/HUD.h"

#include <fstream>
#include <iostream>
#include "Utilities/String.h"
#include "KirbyEffect.h"

#define VELOCITY 200
#define FALLMOTIONCHANGE 1.2f
#define JUMPMIN 0.3
#define JUMPMAX 0.8

KirbyCharacter::KirbyCharacter(Vector3 position, Vector3 size)
	:AnimationRect(position, size, false)
{
	Animator* tempAnimator = new Animator();

	{
		for (size_t i = 0; i < 11; i++)
		{
			Texture2D* srcTex = new Texture2D(TexturePath + L"kirbyAnim/" + animationPng[i]);
			AnimationClip* WalkR = new AnimationClip(motions[i], srcTex, split[i],
				Vector2(0, srcTex->GetHeight() * 0.0f),
				Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 1.0f));
			tempAnimator->AddAnimClip(WalkR);
			SAFE_DELETE(srcTex);
		}

		tempAnimator->SetCurrentAnimClip(L"WalkR");
		SetAnimator(tempAnimator);
	}
	list.push_back(new Rect(position, size/2, 0.0f));//idle, default
	list.push_back(new Rect(position, size * 3 / 4, 0.0f));//floating

	list.push_back(new Rect(position + Values::UpVec * 8, Vector3(size.x / 2.0f, size.y * 0.6875f, size.z), 0.0f));//flyup2
	list.push_back(new Rect(position + Values::UpVec * 8, Vector3(size.x * 0.6875f, size.y * 0.6875f, size.z), 0.0f));//flyup3

	list.push_back(new Rect(position, Vector3(size.x / 2.0f, size.y / 4.0f, size.z), 0.0f));//flat
	list.push_back(new Rect(position, Vector3(size.x / 4.0f, size.y / 2.0f, size.z), 0.0f));//sandwich

	for (size_t i = 0; i < list.size(); i++)
	{
		list[i]->SetColor(Color(0.5f, 0.5f, 0.5f, 0.7f));
	}

	rect = list[0];
	effect = new KirbyEffect();
	effect1 = new KirbyEffect();
}

KirbyCharacter::~KirbyCharacter()
{
	SAFE_DELETE(effect1);
	SAFE_DELETE(effect);
	//SAFE_DELETE(rect);

	for (auto& tmp : list) {
		SAFE_DELETE(tmp);
	}
	list.clear();
}

void KirbyCharacter::Update()
{
	//if flatten state adjust rect position
	Vector3 rectPos = position;
	if (state == flatten) {
		rectPos += Values::DownVec * 15;
	}
	else if (state == flyup) {
		uint curFrame = __super::GetAnimator()->GetCurrentFrameIndex();
		if (curFrame == 2) {
			rectPos += Values::UpVec * 12;
		}
		else if (curFrame == 3) {
			rectPos += Values::UpVec * 12;
		}
	}
	else if (state == exhaling) {
		uint curFrame = __super::GetAnimator()->GetCurrentFrameIndex();
		if (curFrame == 0) {
			rectPos += Values::UpVec * 12;
		}
		else if (curFrame == 1) {
			rectPos += Values::UpVec * 12;
		}
	}
	else if (state == inhaling) {
		rectPos += Values::UpVec * 12;
	}
	else if (state == stopInhaling) {
		rectPos = Values::DownVec * 12;
	}
	rect->SetPosition(rectPos);
	rect->Update();
	__super::Update();
}

void KirbyCharacter::Render()
{
	rect->Render();
	effect->RenderEffect();
	effect1->RenderEffect();
	__super::Render();
}

void KirbyCharacter::Move()
{
	auto key = Keyboard::Get();
	float delta = Time::Delta();
	dir = Values::ZeroVec3;
	
	ChangeBoundingBox();
	Move1(Time::Delta(), key);//right left walk, idle
	if (Move2(Time::Delta(), key)) {//s key exhale, slide, squash down
		return;
	}
	if (Move3(Time::Delta(), key)) {//fly up, jump
		return;
	}
	
	ChangeAnimation(current, VELOCITY * delta, dir, 0, false);
}


void KirbyCharacter::ChangeBoundingBox()
{
	if (state == inhaled) {
		rect = list[1];
	}
	else if (state == flyup) {
		uint curFrame = __super::GetAnimator()->GetCurrentFrameIndex();
		if (curFrame == 2) {
			rect = list[2];
		}
		else if (curFrame == 3) {
			rect = list[3];
		}
		else {
			rect = list[0];
		}
	}
	else if (state == exhaling) {
		uint curFrame = __super::GetAnimator()->GetCurrentFrameIndex();
		if (curFrame == 0) {
			rect = list[3];
		}
		else if (curFrame == 1) {
			rect = list[2];
		}
		else {
			rect = list[0];
		}
	}
	else if (state == sandwiched) {
		rect = list[5];
		float tmp = position.x;
		if (hitLeft) {
			position.x -= 15;
		}
		else if (hitRight) {
			position.x += 15;
		}
		else {
			position.x = tmp;
		}
	}
	else if (state == inhaling) {
		rect = list[2];
		position.y -= 12;
	}
	else if (state == stopInhaling) {
		rect = list[0];
	}
	else if (state == flatten) {
		rect = list[4];
	}
	else {
		rect = list[0];
	}
}

State KirbyCharacter::GetState()
{
	return state;
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

bool KirbyCharacter::Move1(float delta, class Keyboard* key)
{
	bool endDash = EndDash(delta, key);
	bool checkDash = false;
	bool checkWalk = false;
	bool checkHeadDown = false;

	if (!endDash) {
		checkDash = Dash(delta, key);
	}
	if (!endDash && !checkDash) {
		checkWalk = Walk(delta, key);
	}
	
	if (!endDash && !checkDash && !checkWalk) {
		checkHeadDown = HeadDown(delta, key);
	}
	//If not walking or head down state return to idle state
	if (!endDash && !checkDash && !checkWalk && (!checkHeadDown)) {
		WalkingToIdle(delta, key);
	}
	return false;
}

bool KirbyCharacter::Move2(float delta, class Keyboard* key)
{
	if (EatIdle(delta, key)) {
		return true;
	}
	if (EatAndWalk(delta, key)) {
		return true;
	}
	if (Swallowing(delta, key)) {
		return true;
	}
	if (StopInhaling(delta, key)) {
		return true;
	}
	if (Inhaling(delta, key)) {
		return true;
	}
	if (Inhale(delta, key)) {
		return true;
	}
	if (SandWiched(delta, key)) {
		return true;
	}
	if (Drift(delta, key)) {
		return true;
	}
	if (Exhaled(delta, key)) {
		return true;
	}
	if (Exhaling(delta, key)) {
		return true;
	}
	if (Flatten(delta, key)) {
		return true;
	}
	if (Bounce(delta, key)) {
		return true;
	}
	//headdown state can change to squashdown or slide
	//whether you press s key at the same time or not
	if (Slide(delta, key)) {
		return true;
	}
	if (SquashDown(delta, key)) {
		return true;
	}

	if (Exhale(delta, key)) {
		return true;
	}
	return false;
}

bool KirbyCharacter::Move3(float delta, class Keyboard* key)
{
	if (FloatUp(delta, key)) {
		return true;
	}

	if (FlyUp(delta, key)) {
		return true;
	}

	if (StartFly(delta, key)) {
		return true;
	}
	if (Inhaled(delta, key)) {
		return true;
	}
	if (FallDown(delta, key)) {
		return true;
	}
	if (JumpDown(delta, key)) {
		return true;
	}
	if (JumpMin(delta, key)) {
		return true;
	}
	if (JumpLong(delta, key)) {
		return true;
	}
	if (JumpEnd(delta, key)) {
		return true;
	}
	if (JumpStart(delta, key)) {
		return true;
	}
	if (JumpUp(delta, key)) {
		return true;
	}
	if (Idle(delta, key)) {
		return true;
	}
	return false;
}

bool KirbyCharacter::Inhale(float delta, Keyboard* key)
{
	if (state != inhale && hitGround && (state < 2 && key->Press('S'))) {
		state = inhale;
		current = L"inhale";
		dir.x = 0;
		startInhale = Time::Get()->Running();
		return true;
	}
	return false;
}

bool KirbyCharacter::Inhaled(float delta, Keyboard* key)
{
	//kirby has inhaled air to float around
	if (state == inhaled) {
		current = L"inhaled";
		dir += Values::DownVec;
		__super::GetAnimator()->SetPlayRate(current, 1.0f / 10.0f);
		ChangeAnimation(current, VELOCITY * delta, dir, 0, false);
		return true;
	}
	return false;
}

bool KirbyCharacter::Inhaling(float delta, Keyboard* key)
{
	//inhaling first motion
	if (hitGround && state == inhale) {
		if (Time::Get()->Running() - startInhale > 0.1f) {
			state = inhaling;
			effect->SetKirbyEat();
			return true;
		}
		effect->SetKirbyPos(position, __super::GetLeft());
		dir.x = 0;
		dir.y = 0;
		ChangeAnimation(current, VELOCITY * delta, dir, 0, true);
		return true;
	}

	//inhaling second motion
	if (state == inhaling) {
		current = L"inhale";
		dir.x = 0;
		dir.y = 0;		
		//start kirby Effect of inhaling
		effect->SetKirbyPos(position, __super::GetLeft());
		effect->UpdateEffect(Time::Get()->Delta());
		//if user is pressing the key extend the period of inhaling
		if (key->Press('S')) {
			startInhale = Time::Get()->Running();
		}
		
		if (Time::Get()->Running() - startInhale > 0.1f) {
			state = stopInhaling;
			stopInhale = Time::Get()->Running();
			effect->StopEffect();
			return true;
		}
		ChangeAnimation(current, VELOCITY * delta, dir, 1, true);
		return true;
	}
	return false;
}

bool KirbyCharacter::Exhale(float delta, Keyboard* key)
{
	//press s when inhaled for 0.1 second, change to exhaling state
	if (state == inhaled && Time::Get()->Running() - startInhaled > 0.1f
		&& key->Press('S')) {
		current = L"exhaling";
		state = exhaling;
		ChangeAnimation(current, VELOCITY * delta, dir, 0, false);
		return true;
	}
	return false;
}

bool KirbyCharacter::Exhaled(float delta, Keyboard* key)
{
	if (state == exhaled) {
		current = L"exhaling";
		dir += Values::DownVec / 1.6f;
		if (Time::Get()->Running() - delayAfterExhale > 0.1f) {
			state = falldown;//finish exhale motion
			startFalling = Time::Get()->Running();
			return true;
		}
		ChangeAnimation(current, VELOCITY * delta, dir, 3, true);
		return true;
	}
	return false;
}

bool KirbyCharacter::Exhaling(float delta, Keyboard* key)
{
	if (state == exhaling) {
		current = L"exhaling";
		auto curframe = __super::GetAnimator()->GetCurrentFrameIndex();
		if (curframe == 3) {
			state = exhaled;
			delayAfterExhale = Time::Get()->Running();
			return true;
		}
		ChangeAnimation(current, VELOCITY * delta, dir, 0, false);
		return true;
	}
	return false;
}

bool KirbyCharacter::Walk(float delta, Keyboard* key)
{
	if (key->Press(VK_RIGHT)) {
		__super::GetAnimator()->SetPlayRate(current, 1.0f / 10.0f);
		dir += Values::RightVec;
		//block left and right while inhaling motion
		if (state != inhaling) {
			__super::SetLeft(false);
		}

		if (state == eatidle) {
			state = eatandwalk;
			dir = Values::RightVec;
		}
		else if (state == idle) {
			current = L"WalkR";
			state = walking;
		}
		else if (!hitGround && state == walking) {
			state = falldown;
			startFalling = Time::Get()->Running();
		}
		else if (hitGround && hitRight && 
			(prevkirbyX != position.x) && state == walking) {
			state = sandwiched;
			startSandwich = Time::Get()->Running();
			prevkirbyX = position.x;
		}
		return true;
	}
	else if (key->Press(VK_LEFT)) {
		__super::GetAnimator()->SetPlayRate(current, 1.0f / 10.0f);
		dir += Values::LeftVec;
		if (state != inhaling) {
			__super::SetLeft(true);
		}
		if (state == eatidle) {
			state = eatandwalk;
			dir = Values::LeftVec;
		}
		else if (state == idle) {
			current = L"WalkR";
			state = walking;
		}
		else if (!hitGround && state == walking) {
			state = falldown;
			startFalling = Time::Get()->Running();
		}
		else if (hitGround && hitLeft && 
			(prevkirbyX != position.x) && state == walking) {
			state = sandwiched;
			startSandwich = Time::Get()->Running();
			prevkirbyX = position.x;
		}
		return true;
	}
	if (!hitLeft || !hitRight) {
		prevkirbyX = position.x;
	}
	return false;
}

bool KirbyCharacter::Run(float delta, Keyboard* key)
{
	return false;
}

bool KirbyCharacter::HeadDown(float delta, Keyboard* key)
{
	if (hitGround && state != inhaled && 
		state != inhaling && key->Press(VK_DOWN)) {
		dir = Values::ZeroVec3;
		current = L"slide";
		state = headdown;
		return true;
	}
	return false;
}

bool KirbyCharacter::Slide(float delta, Keyboard* key)
{
	//slide when s pressed during heads down state
	if (state == headdown && key->Press('S')) {
		//when sliding check if kirby is on the ground
		if (!hitGround) {
			state = falldown;
			startFalling = Time::Get()->Running();
			return true;
		}
		dir = Values::ZeroVec3;
		if (__super::GetLeft()) {
			dir += Values::LeftVec;
		}
		else {
			dir += Values::RightVec;
		}
		current = L"slide";
		state = headdown;
		ChangeAnimation(current, 2 * VELOCITY * delta, dir, 1, true);
		return true;
	}
	return false;
}

bool KirbyCharacter::WalkingToIdle(float delta, Keyboard* key)
{
	if (state == eatandwalk && hitGround) {
		state = eatidle;
		return true;
	}
	if (state == walking && hitGround) {
		state = idle;
		current = L"Idle";
		return true;
	}
	return false;
}

bool KirbyCharacter::Flatten(float delta, Keyboard* key)
{
	if (state == flatten) {
		dir = Values::ZeroVec3;
		current = L"slide";
		if (Time::Get()->Running() - startSqueeze > 0.1) {
			state = idle;
		}
		ChangeAnimation(current, VELOCITY * delta, dir, 0, true);
		return true;
	}
	return false;
}

bool KirbyCharacter::Bounce(float delta, Keyboard* key)
{
	if (state == bounce) {
		float elapsed = Time::Get()->Running() - startBounce;
		dir.y = 0;

		if (elapsed <= 0.2f) {
			dir += Values::UpVec;
		}
		else if (elapsed <= 0.5f) {
			dir -= Values::UpVec;
		}
		else if (elapsed <= 0.7f) {
			if (!hitGround) {
				state = falldown;
				return true;
			}
			dir += Values::DownVec;
		}
		else {
			state = flatten;
			startSqueeze = Time::Get()->Running();
		}
		current = L"jump";
		ChangeAnimation(current, 1.5f * VELOCITY * delta, dir, 5, true);
		return true;
	}
	return false;
}

bool KirbyCharacter::SquashDown(float delta, Keyboard* key)
{
	if (state == headdown) {
		ChangeAnimation(current, 0, dir, 0, true);
		state = idle;
		return true;
	}
	return false;
}

bool KirbyCharacter::FloatUp(float delta, Keyboard* key)
{	
	//while inhaled kirby can float upwards
	if (state == inhaled && (key->Press(VK_UP) || key->Press('Z'))) {
		current = L"inhaled";
		dir += Values::UpVec;
		__super::GetAnimator()->SetPlayRate(current, 1.0f / 20.0f);
		ChangeAnimation(current, VELOCITY * delta, dir, 0, false);
		return true;
	}
	return false;
}

bool KirbyCharacter::FlyUp(float delta, Keyboard* key)
{
	if (state == flyup) {
		dir += Values::UpVec;
		current = L"flyUp";
		state = flyup;
		auto curframe = __super::GetAnimator()->GetCurrentFrameIndex();
		if (curframe == 3) {
			state = inhaled;
			startInhaled = Time::Get()->Running();
		}
		ChangeAnimation(current, VELOCITY * delta, dir, 0, false);
		return true;
	}
	return false;
}

bool KirbyCharacter::StartFly(float delta, Keyboard* key)
{
	if (state != exhaling && key->Press(VK_UP)) {
		current = L"flyUp";
		state = flyup;
		ChangeAnimation(current, VELOCITY * delta, dir, 0, false);
		return true;
	}
	return false;
}

bool KirbyCharacter::FallDown(float delta, Keyboard* key)
{
	if (state == falldown) {
		//when you hit ground while falling
		if (hitGround) {
			//when you fall from higher up bounce once
			if (Time::Get()->Running() - startFalling > FALLMOTIONCHANGE) {
				state = bounce;
				current = L"jump";
				__super::GetAnimator()->SetPlayRate(current, 1.0f / 10.0f);
				startBounce = Time::Get()->Running();
				return true;
			}
			else {
				state = flatten;
				current = L"slide";
				__super::GetAnimator()->SetPlayRate(current, 1.0f / 10.0f);
				startSqueeze = Time::Get()->Running();
				return true;
			}
		}
		dir += Values::DownVec;
		current = L"jump";
		if (Time::Get()->Running() - startFalling < FALLMOTIONCHANGE) {
			ChangeAnimation(current, 2 * VELOCITY * delta, dir, 4, true);
			return true;
		}
		else {
			ChangeAnimation(current, 2 * VELOCITY * delta, dir, 5, true);
			return true;
		}
	}
	return false;
}

bool KirbyCharacter::JumpDown(float delta, Keyboard* key)
{
	if (state == jumpdown) {
		dir += Values::DownVec;
		current = L"jump";
		if (hitGround) {
			startSqueeze = Time::Get()->Running();
			state = flatten;
			return true;
		}
		uint curIdx = __super::GetAnimator()->GetCurrentFrameIndex();
		if (curIdx == 4) {
			ChangeAnimation(current, 2 * VELOCITY * delta, dir, 4, true);
			return true;
		}
		ChangeAnimation(current, 2 * VELOCITY * delta, dir, 0, false);
		return true;
	}
	return false;
}

bool KirbyCharacter::JumpMin(float delta, Keyboard* key)
{
	if (state == jumpmin) {
		if (Time::Get()->Running() - startJump < JUMPMIN) {
			current = L"jump";
			dir.y = 0;
			dir += Values::UpVec;
			ChangeAnimation(current, 2 * VELOCITY * delta, dir, 4, true);
			return true;
		}
		else {
			state = jumpdown;
			return true;
		}
	}
	return false;
}

bool KirbyCharacter::JumpLong(float delta, Keyboard* key)
{
	//if player pressed for too long, end the jump
	if (state == jump && (Time::Get()->Running() - startJump > JUMPMAX)) {
		state = jumpdown;
		return true;
	}
	return false;
}

bool KirbyCharacter::JumpEnd(float delta, Keyboard* key)
{
	//when user wants to end jump
	if (key->Up('Z')) {
		//grant minimum jump for short press
		if (state == jump && (Time::Get()->Running() - startJump < JUMPMIN)) {
			state = jumpmin;
		}
		//end the jump if jump time is in range of jumpmin and jumpmax
		else if (state == jump && (Time::Get()->Running() - startJump <= JUMPMAX)) {
			state = jumpdown;
		}
		return true;
	}
	return false;
}

bool KirbyCharacter::JumpStart(float delta, Keyboard* key)
{
	//jump when kirby hits the ground
	if (key->Down('Z') && hitGround) {
		state = jump;
		current = L"jump";
		startJump = Time::Get()->Running();
		return true;
	}
	return false;
}

bool KirbyCharacter::JumpUp(float delta, Keyboard* key)
{
	if (state == jump) {
		current = L"jump";
		dir.y = 0;
		dir += Values::UpVec;
		ChangeAnimation(current, 2 * VELOCITY * delta, dir, 0, true);
		return true;
	}
	return false;
}

bool KirbyCharacter::Idle(float delta, Keyboard* key)
{
	if (state == idle)
	{
		current = L"Idle";
		state = idle;
		__super::GetAnimator()->SetPlayRate(current, 1.0f / 10.0f);
		ChangeAnimation(current, 0, dir, 0, false);
		return true;
	}
	return false;
}

bool KirbyCharacter::Dash(float delta, Keyboard* key)
{
	if (hitGround && state != inhaled && state != eatandwalk) {
		//if there was a right key down before
		if (prevRight && Time::Get()->Running() - prevRightTime < 0.3f && 
			key->Down(VK_RIGHT)) {
			state = dash;
			return true;
		}
		//if there was a left key down before
		else if (prevLeft && Time::Get()->Running() - prevLeftTime < 0.3f && 
			key->Down(VK_LEFT)) {
			state = dash;
			return true;
		}
		//first time pressing right key
		else if (key->Down(VK_RIGHT)) {
			prevRightTime = Time::Get()->Running();
			prevRight = true;
			prevLeft = false;
		}
		//first time pressing left key
		else if (key->Down(VK_LEFT)) {
			prevLeftTime = Time::Get()->Running();
			prevLeft = true;
			prevRight = false;
		}
	}
	else {
		//if kirby is not on the ground reset
		prevRight = false;
		prevLeft = false;
		if (state == dash) {
			//reset animation speed
			state = falldown;
			startFalling = Time::Get()->Running();
			return true;
		}
	}

	if (state == dash) {
		__super::GetAnimator()->SetPlayRate(current, 1.0f / 20.0f);
		if (prevRight) {
			//right dash
			__super::SetLeft(false);
			current = L"WalkR";
			//if hit the wall while dash
			if (hitGround && hitRight) {
				state = sandwiched;
				startSandwich = Time::Get()->Running();
				return true;
			}

			//when key press up finish dash slowly
			if (key->Up(VK_RIGHT)) {
				state = endDash;
				endDashTime = Time::Get()->Running();
				return true;
			}
			dir += Values::RightVec;
			ChangeAnimation(current, 2 * VELOCITY * delta, dir, 0, false);
		}
		else {
			//left dash
			__super::SetLeft(true);
			current = L"WalkR";

			if (hitGround && hitLeft) {
				state = sandwiched;
				startSandwich = Time::Get()->Running();
				return true;
			}
			//when key press up finish dash slowly
			if (key->Up(VK_LEFT)) {
				state = endDash;
				endDashTime = Time::Get()->Running();
				return true;
			}
			dir += Values::LeftVec;
			ChangeAnimation(current, 2 * VELOCITY * delta, dir, 0, false);
		}
		return true;
	}
	return false;
}

bool KirbyCharacter::EndDash(float delta, Keyboard* key)
{
	//slow down at the end of dash	
	if (state == endDash) {
		float elapsed = Time::Get()->Running() - endDashTime;

		//first walk for 0.2 second reducing velocity
		if (elapsed < 0.2f) {
			if (prevRight) {
				//drift if press left key while dashing right
				if (key->Down(VK_LEFT)) {
					state = drift;
					current = L"jump";
					__super::GetAnimator()->SetCurrentAnimClip(current);
					__super::GetAnimator()->SetCurrentFrame(2);
				}
				else {
					current = L"WalkR";
					__super::GetAnimator()->SetPlayRate(current, 1.0f / 10.0f);
				}
				return true;
			}
			else if (prevLeft) {
				if (key->Down(VK_RIGHT)) {
					state = drift;
					current = L"jump";
					__super::GetAnimator()->SetCurrentAnimClip(current);
					__super::GetAnimator()->SetCurrentFrame(2);
				}
				else {
					current = L"WalkR";
					__super::GetAnimator()->SetPlayRate(current, 1.0f / 10.0f);
				}
				return true;
			}
			return true;
		}
		//then change to idle motion
		else {
			prevLeft = false;
			prevRight = false;
			state = idle;
			current = L"Idle";
			return true;
		}
	}
	return false;
}

bool KirbyCharacter::Drift(float delta, Keyboard* key)
{
	if (state == drift) {
		float elapsed = Time::Get()->Running() - endDashTime;
		if (elapsed < 0.3f) {
			if (prevLeft) {
				__super::SetLeft(true);
				dir.x = 0;
				dir += Values::LeftVec;
			}
			else {
				__super::SetLeft(false);
				dir.x = 0;
				dir += Values::RightVec;
			}
			current = L"jump";
			__super::GetAnimator()->SetPlayRate(current, 1.0f / 10.0f);
			ChangeAnimation(current, 2 * VELOCITY * delta * (0.3f - elapsed), dir, 3, true);

			return true;
		}
		else {
			state = idle;
			return true;
		}
	}
	return false;
}

bool KirbyCharacter::SandWiched(float delta, Keyboard* key)
{
	if (state == sandwiched) {
		if (Time::Get()->Running() - startSandwich > 0.1f) {
			state = idle;
			return true;
		}
		current = L"sandwiched";
		__super::GetAnimator()->SetPlayRate(current, 1.0f / 10.0f);
		ChangeAnimation(current, 0, dir, 0, false);
		return true;
	}
	return false;
}

bool KirbyCharacter::StopInhaling(float delta, Keyboard* key)
{
	if (state == stopInhaling) {
		current = L"inhale";
		//for next 0.1 second show second motion
		if (Time::Get()->Running() - stopInhale < 0.1f) {
			ChangeAnimation(current, 0, dir, 0, true);
			return true;
		}
		//if over 0.15 second change to idle
		state = idle;
	}
	return false;
}

bool KirbyCharacter::Swallowing(float delta, Keyboard* key)
{
	if (state == swallowing) {
		dir.x = 0;
		dir.y = 0;	
		
		//start kirby Effect of inhaling
		effect->SetKirbyPos(position, __super::GetLeft());
		effect->UpdateEffect(Time::Get()->Delta());
		//start kirby Effect of swallowing enemy
		effect1->SetKirbyPos(position, __super::GetLeft());

		//finish when all enemies reach p1
		if (effect1->UpdateSwallowEffect()) {
			//kirby idle motion after eat enemy
			state = eatidle;
			effect1->StopEffect();
			effect->StopEffect();
			return true;
		}

		ChangeAnimation(current, VELOCITY * delta, dir, 1, true);
		return true;
	}
	return false;
}

bool KirbyCharacter::EatIdle(float delta, Keyboard* key)
{
	if (state == eatidle) {
		current = L"eatidle";
		dir = Values::ZeroVec3;
		ChangeAnimation(current, VELOCITY * delta, dir, 0, false);
		return true;
	}
	return false;
}

bool KirbyCharacter::EatAndWalk(float delta, Keyboard* key)
{
	if (state == eatandwalk) {
		current = L"eatandwalk";
		ChangeAnimation(current, VELOCITY * delta, dir, 0, false);
		return true;
	}
	return false;
}

void KirbyCharacter::Attack()
{
}

void KirbyCharacter::ApplyGravity()
{
}

void KirbyCharacter::SetEnemySwallowed(vector<class Enemy*>& enemySwallowed)
{
	effect1->SetKirbyPos(position, __super::GetLeft());
	effect1->SetKirbySwallow(enemySwallowed);
}

void KirbyCharacter::ClearEnemySwallowed()
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
