#include "stdafx.h"
#include "Ending.h"
#include "Geomatries/AnimationRect.h"
#include "Geomatries/TextureRect.h"

void Ending::Init()
{
	endingAnimation = new AnimationRect(position, size, false);
	Animator* tempAnimator = new Animator();

	{
		for (size_t i = 0; i < 10; i++)
		{
			wstring num = String::ToWString(to_string(i));
			path = TexturePath + L"ending/" + num + L".png";
			Texture2D* srcTex = new Texture2D(path);
			AnimationClip* tmpClip = new AnimationClip(L"end" + num, srcTex, 10,
				Values::ZeroVec2,
				Vector2(srcTex->GetWidth(), srcTex->GetHeight()), false, 0.1f, true);
			tempAnimator->AddAnimClip(tmpClip);
			SAFE_DELETE(srcTex);
		}

		tempAnimator->SetCurrentAnimClip(L"end0");
		endingAnimation->SetAnimator(tempAnimator);
	}
	path = TexturePath + L"ending/endingCredit.png";
	endingCredit = new TextureRect(position, { 481.0f,114.0f,0.0f }, 0.0f, path);
}

void Ending::Destroy()
{
	SAFE_DELETE(endingAnimation);
	SAFE_DELETE(endingCredit);
}

void Ending::Update()
{
	if (end) {
		endingAnimation->ChangeAnimation(L"end0",0.0f,Values::ZeroVec3,0, true);
		endingAnimation->Update();
		endingCredit->Update();
		return;
	}
	speed++;

	if (curClipIdx == 0 && speed < 200) {
		endingAnimation->ChangeAnimation(L"end0", 0.0f, Values::ZeroVec3, 0, true);
		endingAnimation->Update();
		return;
	}
	else if (curClipIdx == 0 && speed >= 200) {
		speed = 0;
		curClipIdx = 1;
		//timer = Time::Get()->Running();
	}
	clipName = L"end" + String::ToWString(to_string(curClipIdx - 1));
	endingAnimation->ChangeAnimation(clipName, 0.0f, Values::ZeroVec3, 0, false);
	endingAnimation->Update();
	curFrame = endingAnimation->GetAnimator()->GetCurrentFrameIndex();
	if (curFrame == 9 && curClipIdx < clipTotal) {
		curClipIdx++;
		curFrame = 0;
		//timer = Time::Get()->Running();
	}
	if (curFrame == 9 && curClipIdx == clipTotal) {
		end = true;
	}
}

void Ending::Render()
{
	if (end) {
		endingAnimation->Render();
		endingCredit->Render();
		return;
	}

	endingAnimation->Render();
}

void Ending::PostRender()
{
}

void Ending::GUI()
{
}
