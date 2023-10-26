#include "stdafx.h"
#include "Geomatries/Rect.h"
#include "Geomatries/AnimationRect.h"
#include "Utilities/Animator.h"
#include "Boss.h"

TreeBoss::TreeBoss(Vector3 position, Vector3 size) :
    AnimationRect(position, size, false)
{
    Animator* tempAnimator = new Animator();
    for(int i = 0; i < BOSSPNGNUM; i++){

        Texture2D* srcTex = new Texture2D(BossPath + png[i]);
        AnimationClip* animClip = new AnimationClip(png[i], srcTex, split[i],
            Vector2(0, srcTex->GetHeight() * 0.0f),
            Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 1.0f));

        tempAnimator->AddAnimClip(animClip);
        SAFE_DELETE(srcTex);
    }
    tempAnimator->SetCurrentAnimClip(png[0]);
    SetAnimator(tempAnimator);

    //set bounding box for enemy
    rect = new Rect(position, size, 0.0f);
}

TreeBoss::~TreeBoss()
{
}

void TreeBoss::Update()
{
    rect->Update();
    ChangeAnimation(current, 0.0f, Values::ZeroVec3, 0, false);

    __super::Update();
}

void TreeBoss::Render()
{
    __super::Render();
    rect->Render();

}

void TreeBoss::Move()
{
}

void TreeBoss::Idle()
{
}

void TreeBoss::AppleAtk()
{
}

void TreeBoss::Attack()
{
}

void TreeBoss::SetHit()
{
}

void TreeBoss::SetDeath()
{
}

void TreeBoss::SetAnimator(class Animator* animator)
{
    __super::SetAnimator(animator);
}
