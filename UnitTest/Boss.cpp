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

    endingItem = new AnimationRect({position.x, endingPosY, 0.0f}, 
                                    Vector3(96.0f, 96.0f, 0.0f), 0.0f);
    endingRect = new Rect({ position.x, endingPosY, 0.0f },
        Vector3(96.0f, 96.0f, 0.0f), 0.0f);
    Animator* tempAnimator1 = new Animator();
    //create ending item
    {
        Texture2D* srcTex = new Texture2D(BossPath + L"endingItem.png");
        AnimationClip* tmpClip = new AnimationClip(L"endingItem.png", srcTex, 48,
            Vector2(0, srcTex->GetHeight() * 0.0f),
            Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 1.0f), false, 0.01f);
        //add clip for each animator
        tempAnimator1->AddAnimClip(tmpClip);
        tempAnimator1->SetCurrentAnimClip(L"endingItem.png");
        SAFE_DELETE(srcTex);
    }
    endingItem->SetAnimator(tempAnimator1);
    endingPos = endingItem->GetPosition();
}

TreeBoss::~TreeBoss()
{
    SAFE_DELETE(rect);
    SAFE_DELETE(endingItem);
    SAFE_DELETE(endingRect);
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
        ending = true;
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
        if (ending) {
            if (endingPos.x > endingPosX) {
                endingPos.x -= 2.0f;
            }
            endingItem->SetPosition(endingPos);
            endingItem->ChangeAnimation(L"", 0.0f, Values::ZeroVec3, 0, false);
            endingItem->Update();

            endingRect->SetPosition(endingPos);
            endingRect->Update();
        }

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
    if (ending) {
        endingItem->Render();
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

void TreeBoss::SetHealth(int health)
{
    if (health < 0) {
        health = 0;
    }
    this->health = health;
}

void TreeBoss::SetAnimator(class Animator* animator)
{
    __super::SetAnimator(animator);
}
