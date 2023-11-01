#include "stdafx.h"
#include "Intro.h"
#include "Geomatries/AnimationRect.h"
#include "Utilities/Animator.h"

void Intro::Init()
{
	animation = new AnimationRect(position, size, false);
	Animator* tempAnimator = new Animator();

	{
		Texture2D* srcTex = new Texture2D(TexturePath + L"intro/intro.png");
		AnimationClip* intro1 = new AnimationClip(L"intro1", srcTex, 8,
			Vector2(0, srcTex->GetHeight() * 0.0f),
			Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 0.333333f));
		AnimationClip* intro2 = new AnimationClip(L"intro2", srcTex, 8,
			Vector2(0, srcTex->GetHeight() * 0.333333f),
			Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 0.666667f));
		AnimationClip* intro3 = new AnimationClip(L"intro3", srcTex, 7,
			Vector2(0, srcTex->GetHeight() * 0.666667f),
			Vector2(srcTex->GetWidth() * 0.875f, srcTex->GetHeight() * 1.0f));
		tempAnimator->AddAnimClip(intro1);
		tempAnimator->AddAnimClip(intro2);
		tempAnimator->AddAnimClip(intro3);
		SAFE_DELETE(srcTex);

		tempAnimator->SetCurrentAnimClip(L"intro1");
		animation->SetAnimator(tempAnimator);
	}
}


void Intro::Destroy()
{
}

void Intro::Update()
{
	if (idx == 0) {
		animation->ChangeAnimation(L"intro1",0.0f,Values::ZeroVec3, 0, false);
		if (animation->GetAnimator()->GetCurrentFrameIndex() == 7) {
			idx++;
		}
		animation->Update();
		return;
	}
	else if (idx == 1) {
		animation->ChangeAnimation(L"intro2", 0.0f, Values::ZeroVec3, 0, false);
		if (animation->GetAnimator()->GetCurrentFrameIndex() == 7) {
			idx++;
		}
		animation->Update();
		return;
	}
	else if (idx == 2) {
		animation->ChangeAnimation(L"intro3", 0.0f, Values::ZeroVec3, 0, false);
		if (animation->GetAnimator()->GetCurrentFrameIndex() == 6) {
			idx++;
		}
		animation->Update();
		return;
	}
	else{
		animation->ChangeAnimation(L"intro3", 0.0f, Values::ZeroVec3, 6, true);
		animation->Update();
		return;
	}
}

void Intro::Render()
{
	animation->Render();
}

void Intro::PostRender()
{
}

void Intro::GUI()
{
}
