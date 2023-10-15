#include "stdafx.h"
#include "Geomatries/Rect.h"
#include "Geomatries/AnimationRect.h"
#include "Enemy.h"
#include "EnemyInfo.h"
#include <fstream>
#include <sstream>

#define VELOCITY 200

void Enemy::setKirbyPos(Vector3 pos)
{
    this->kirbyPos = pos;
}


void Enemy::attackPlayer()
{
}

void Enemy::moveTowardsPlayer()
{
}

Enemy::Enemy(Vector3 position, Vector3 size, string name, class EnemyInfo* infos)
    : AnimationRect(position, size, false)
{
    this->infos = infos;
    Animator* tempAnimator = new Animator();
    vector<EnemyData> data =  infos->GetData();
    for (size_t i = 0; i < data.size(); i++)
    {
        //found the same name of enemy from Enemy info
        if (name.compare(data[i].name) == 0) {
            clipname = String::ToWString(data[i].name);
            wstring tmpPath = L"kirbyEnemy/" + clipname + L".png";
            Texture2D* srcTex = new Texture2D(TexturePath + tmpPath);
            AnimationClip* animClip = new AnimationClip(clipname, srcTex, data[i].split,
                Vector2(0, srcTex->GetHeight() * 0.0f),
                Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 1.0f));
            
            tempAnimator->AddAnimClip(animClip);
            tempAnimator->SetCurrentAnimClip(clipname);
            SetAnimator(tempAnimator);

            SAFE_DELETE(srcTex);
            break;
        }
    }
    //set bounding box for enemy
    rect = new Rect(position, size / 2, 0.0f);
    //set color to see gray bounding box
    rect->SetColor(Color(0.5f, 0.5f, 0.5f, 0.7f));
    idleTimer = Time::Get()->Running();
}

Enemy::~Enemy()
{
    SAFE_DELETE(rect);
    
}

void Enemy::Update()
{
    switch (state) {
    case IDLE:
        MoveIdle();
        if (playerInRange(inRange)) {
            state = CHASE;
        }
        break;
    case CHASE:
        if (playerOutOfRange(outRange)) {
            state = IDLE;
        }
        else if (playerInRange(inRange) && canAttack(attackRange)) {
            state = ATTACK;
        }
        else {
            moveTowardsPlayer();
        }
        break;
    case ATTACK:
        if (playerOutOfRange(outRange)) {
            state = IDLE;
        }
        else if (!canAttack(attackRange)) {
            state = CHASE;
        }
        else {
            attackPlayer();
        }
        break;
    }
    rect->SetPosition({ position.x, position.y, 0});
    rect->Update();
    __super::Update();
}

void Enemy::Render()
{
    rect->Render();
    __super::Render();
}
//left, right
void Enemy::Move()
{
    __super::Move();
}

void Enemy::SetAnimator(Animator* animator)
{
    __super::SetAnimator(animator);
}

void Enemy::SetPosition(Vector3 pos)
{
    rect->SetPosition(pos);
    __super::SetPosition(pos);
}


Rect* Enemy::GetRect()
{
    return rect;
}

void Enemy::SetEnemyInfo(class EnemyInfo* infos)
{
    this->infos = infos;
}

void Enemy::Walk()
{
    int velocity = Time::Delta() * VELOCITY;
    Vector3 dir = Values::RightVec;
    if (!__super::GetLeft()) {
        dir = Values::LeftVec;
    }
    //if not hit ground
    if (!hitGround) {
        dir += Values::DownVec;
    }
    ChangeAnimation(this->clipname, velocity, dir, 0, false);
}

void Enemy::Jump()
{
}

void Enemy::Fly()
{
}

void Enemy::MoveIdle()
{
    if (hitLeft) {
        move = WALKRIGHT;
    }
    else if(hitRight){
        move = WALKLEFT;
    }
    if (move == WALKLEFT) {
        __super::SetLeft(false);
        Walk();
    }
    else {
        __super::SetLeft(true);
        Walk();
    }

}

void Enemy::AttackPlayer()
{
}

void Enemy::rangedAttackPlayer()
{
}
