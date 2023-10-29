#include "stdafx.h"
#include "Geomatries/Rect.h"
#include "Geomatries/AnimationRect.h"
#include "Utilities/Animator.h"
#include "Boss.h"
#include "KirbyEffect.h"

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
    hitPositionY = position.y + 21.0f;
    defaultPos = position.y;
    //set bounding box for enemy
    rect = new Rect(position + Values::RightVec * 40, size, 0.0f);
}

TreeBoss::~TreeBoss()
{
}

void TreeBoss::Update()
{
    if (rect) {
        rect->Update();
    }

    prevState = state;

    if (state != death && health == 0) {
        state = death;
        timer = Time::Get()->Running();
        SAFE_DELETE(rect);
    }

    switch (state)
    {
    case idle:
        position.y = defaultPos;
        if (Time::Get()->Running() - timer > idleTime) {
            if (idleTime > 2.0f) {
                idleTime = 1.5f;
            }
            state = atk;
            timer = Time::Get()->Running();
        }
        break;
    case atk:
        position.y = defaultPos;
        Attack();
        if (Time::Get()->Running() - timer > 1.5f) {
            state = appleatk;
            timer = Time::Get()->Running();
        }
        break;
    case appleatk:
        position.y = defaultPos;
        AppleAtk();
        if (Time::Get()->Running() - timer > 11.0f) {
            state = idle;
            timer = Time::Get()->Running();
        }
        break;
    case hit:
        current = png[3];
        position.y = hitPositionY;
        if (Time::Get()->Running() - hitTimer > 0.6f) {
            state = saveState;//recall the saved state
            return;
        }
        break;
    case death:
        position.y = defaultPos;
        current = png[2];
        if (Time::Get()->Running() - timer > 0.4f) {
            ChangeAnimation(current, 0.0f, Values::ZeroVec3, 1, true);
            __super::Update();
            return;
        }
        else {
            ChangeAnimation(current, 0.0f, Values::ZeroVec3, 0, true);
            __super::Update();
            return;
        }
    default:
        break;
    }
    ChangeAnimation(current, 0.0f, Values::ZeroVec3, 0, false);
    __super::Update();
}

void TreeBoss::Render()
{
    __super::Render();
    if (rect) {
        rect->Render();
    }
}

void TreeBoss::Move()
{
}

void TreeBoss::AppleAtk()
{
    current = png[0];
   //dir = Values::DownVec;
}

void TreeBoss::Attack()
{
    current = png[1];
}


void TreeBoss::SetDeath()
{
}

int TreeBoss::GetState()
{
    return state;
}

void TreeBoss::SetState(int state)
{
    this->state = (BossState)state;
}

void TreeBoss::SaveState()
{
    this->saveState = state;
}

void TreeBoss::SetHitTimer()
{
    hitTimer = Time::Get()->Running();
    if (health > 0) {
        health--;
    }
}

int TreeBoss::GetPrevState()
{
    return prevState;
}

Rect* TreeBoss::GetRect()
{
    return rect;
}

void TreeBoss::SetTimer()
{
    this->timer = Time::Get()->Running();
}

void TreeBoss::SetAnimator(class Animator* animator)
{
    __super::SetAnimator(animator);
}
