#include "stdafx.h"
#include "Geomatries/Rect.h"
#include "Geomatries/AnimationRect.h"
#include "KirbyCharacter.h"
#include "Utilities/Animator.h"
#include "UI/HUD.h"

#include <fstream>
#include <iostream>
#include "Utilities/String.h"
#define VELOCITY 200
#define FALLMOTIONCHANGE 1.2f
#define JUMPMIN 0.3
#define JUMPMAX 0.8

void Log(const std::string& message)
{
	std::ofstream logFile("log.txt", std::ios_base::app);
	logFile << message << "\n";
	std::cout << message << "\n";
}

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

		Texture2D* srcTex7 = new Texture2D(TexturePath + L"kirbyAnim/kirbySandwiched.png");
		AnimationClip* sandwiched = new AnimationClip(L"sandwiched", srcTex7, 1,
			Values::ZeroVec2,
			Vector2(srcTex7->GetWidth(), srcTex7->GetHeight() * 1.0f));

		tempAnimator->AddAnimClip(WalkR);
		tempAnimator->AddAnimClip(WalkL);
		tempAnimator->AddAnimClip(Idle);
		tempAnimator->AddAnimClip(flyUp);
		tempAnimator->AddAnimClip(inhaled);
		tempAnimator->AddAnimClip(exhaling);
		tempAnimator->AddAnimClip(jump);
		tempAnimator->AddAnimClip(slide);
		tempAnimator->AddAnimClip(sandwiched);

		tempAnimator->SetCurrentAnimClip(L"WalkR");
		SetAnimator(tempAnimator);
		SAFE_DELETE(srcTex2);
		SAFE_DELETE(srcTex1);
		SAFE_DELETE(srcTex);
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
}

KirbyCharacter::~KirbyCharacter()
{
	SAFE_DELETE(rect);
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

	rect->SetPosition(rectPos);
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

void KirbyCharacter::ChangeAnimation(wstring clipName, float speed, Vector3 dir, uint currentFrame, bool setFrame)
{
	string clipname = String::ToString(clipName);
	
	Log("clipname: " + clipname + " ,state: " + to_string(state) + 
		",currentFrame: " + to_string(currentFrame));
	auto myframe = __super::GetAnimator()->GetCurrentFrame();
	Log("frame: " + to_string(myframe.x) + " " + to_string(myframe.y));
	if (setFrame) {
		__super::GetAnimator()->SetCurrentFrame(currentFrame);
	}
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
	else if (state == flatten) {
		rect = list[4];
	}
	else {
		rect = list[0];
	}
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
	return false;
}

Vector3 FindControlPoint(Vector3 p1, Vector3 p2)
{
	return Vector3((p1.x + p2.x) / 2, (p1.y + p2.y) / 2, 0.0f);
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
		ChangeAnimation(current, VELOCITY * delta, dir, 2, true);
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

bool KirbyCharacter::Jump(float delta, Keyboard* key)
{
	return false;
}

bool KirbyCharacter::Walk(float delta, Keyboard* key)
{
	if (key->Press(VK_RIGHT)) {
		__super::GetAnimator()->SetPlayRate(current, 1.0f / 10.0f);
		dir += Values::RightVec;
		__super::SetLeft(false);
		if (state == idle) {
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
		__super::SetLeft(true);
		if (state == idle) {
			current = L"WalkL";
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
	if (hitGround && state != inhaled && key->Press(VK_DOWN)) {
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
		ChangeAnimation(current, 2 * VELOCITY * delta, dir, 0, true);
		return true;
	}
	return false;
}

bool KirbyCharacter::WalkingToIdle(float delta, Keyboard* key)
{
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
		ChangeAnimation(current, 1.5f * VELOCITY * delta, dir, 4, true);
		return true;
	}
	return false;
}

bool KirbyCharacter::SquashDown(float delta, Keyboard* key)
{
	if (state == headdown) {
		ChangeAnimation(current, 0, dir, 1, true);
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
			ChangeAnimation(current, 2 * VELOCITY * delta, dir, 3, true);
			return true;
		}
		else {
			ChangeAnimation(current, 2 * VELOCITY * delta, dir, 4, true);
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
			ChangeAnimation(current, 2 * VELOCITY * delta, dir, 3, true);
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
			ChangeAnimation(current, 2 * VELOCITY * delta, dir, 5, true);
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
		ChangeAnimation(current, 2 * VELOCITY * delta, dir, 5, true);
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
	if (hitGround && state != inhaled) {
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
			current = L"WalkL";

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
					current = L"WalkL";
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
			ChangeAnimation(current, 2 * VELOCITY * delta * (0.3f - elapsed), dir, 2, true);

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
