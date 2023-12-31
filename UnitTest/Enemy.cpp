#include "stdafx.h"

#include "Enemy.h"
#include <fstream>
#include <sstream>

#define VELOCITY 200

void Enemy::SetKirbyPos(Vector3 pos)
{
    this->kirbyPos = pos;
}

string Enemy::GetAbility()
{
    return (infos->GetData())[name].ability;
}

void Enemy::WaitApple(float time)
{
    appleWait = Time::Get()->Running() + time;
}


void Enemy::attackPlayer()
{
    map<string, EnemyData> data = infos->GetData();

    if (data[name].ability.compare("spark") == 0) {
        sparkEffect.UpdateSparkEffect(Time::Delta());
    }
    else if (data[name].ability.compare("beam") == 0) {
        beamEffect.UpdateBeamEffect(Time::Delta(), !GetLeft());
    }
}

void Enemy::moveTowardsPlayer()
{
    //follow kirby pos
    if (position.x <= kirbyPos.x) {
        move = WALKRIGHT;
    }
    else {
        move = WALKLEFT;
    }
    //hit wall
    if (hitLeft) {
        move = WALKRIGHT;
        hitwallTime = Time::Get()->Running();
        hitwall = true;
    }
    else if (hitRight) {
        move = WALKLEFT;
        hitwallTime = Time::Get()->Running();
        hitwall = true;
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

Enemy::Enemy(Vector3 position, Vector3 size, string name, class EnemyInfo* infos)
    : AnimationRect(position, size, false)
{
    this->infos = infos;
    this->name = name;

    Animator* tempAnimator = new Animator();
    map<string, EnemyData> data =  infos->GetData();
    {
        clipname = String::ToWString(name);
        wstring tmpPath = L"kirbyEnemy/" + clipname + L".png";
        Texture2D* srcTex = new Texture2D(TexturePath + tmpPath);
        AnimationClip* animClip = new AnimationClip(clipname, srcTex, data[name].split,
            Vector2(0, srcTex->GetHeight() * 0.0f),
            Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 1.0f));
            
        tempAnimator->AddAnimClip(animClip);
        tempAnimator->SetCurrentAnimClip(clipname);
        SetAnimator(tempAnimator);

        SAFE_DELETE(srcTex);
    }
    //set bounding box for enemy
    rect = new Rect(position, size / 2, 0.0f);
    //set color to see gray bounding box
    idleTimer = Time::Get()->Running();
}

Enemy::~Enemy()
{
    SAFE_DELETE(rect);
    
}

void Enemy::Update()
{
    map<string, EnemyData> data = infos->GetData();

    //cout << state << "\n";
    switch (state) {
    case IDLE:
        if (name.compare("apple") == 0) {
            AppleMove();
            break;
        }
        MoveIdle();
        if (playerInRange(inRange) && data[name].follow) {
            state = CHASE;
        }
        break;
    case CHASE:
        if (hitwall && Time::Get()->Running() - hitwallTime < 1.0f) {
            state = IDLE;
        }
        else if (playerOutOfRange(outRange)) {
            state = IDLE;
        }
        else if (canAttack(attackRange)) {
            state = ATTACK;
            //check if it is waddledoo or sparky
            if (data[name].ability.compare("spark") == 0) {
                sparkEffect.SetSparkEffect(rect->GetPosition());
                attackTime = 4.0f;
                sparkEffect.StartTimer(attackTime);
            }
            else if (data[name].ability.compare("beam") == 0) {
                beamEffect.SetBeamEffect(rect->GetPosition(), !GetLeft());
                attackTime = 2.0f;
                beamEffect.StartTimer(attackTime);
            }

            startAttack = Time::Get()->Running();
        }
        else {
            moveTowardsPlayer();
        }
        if (Time::Get()->Running() - hitwallTime > 1.0f) {
            hitwall = false;
        }
        break;
    case ATTACK:
        if (Time::Get()->Running() - startAttack < attackTime) {
            state = ATTACK;
            attackPlayer();
        }
        else if (playerOutOfRange(outRange)) {
            state = IDLE;
        }
        else if (!canAttack(attackRange)) {
            state = CHASE;
        }
        else {
            attackPlayer();
        }
        break;
    default:
        break;
    }
    if (rect) {
        rect->SetPosition({ position.x, position.y, 0 });
        rect->Update();
        __super::Update();
    }
}

void Enemy::Render()
{
    if (state == ATTACK) {
        map<string, EnemyData> data = infos->GetData();
        //cout << data[name].ability << "\n";
        if (data[name].ability.compare("spark") == 0) {
            sparkEffect.RenderSparkEffect();
        }
        else if (data[name].ability.compare("beam") == 0) {
            beamEffect.RenderBeamEffect();
        }
    }
    if (rect) {
        rect->Render();
    }
    if (appleWait > Time::Get()->Running()) {
        return; //wait for other apples to fall
    }
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

void Enemy::AppleMove()
{
    if (appleWait > Time::Get()->Running()) {
        return; //wait for other apples to fall
    }
    Vector3 dir = Values::ZeroVec3;
    //show blinking image of apple
    if (Time::Get()->Running() > appleWait + 1.5f) {
        dir = Values::DownVec;
    }


    //cout << "appleBounce: " << appleBounce << "\n";
    //cout << position.x << ":" << position.y << "\n";
    int velocity = Time::Delta() * VELOCITY * 2.4f;

    if (bouncing) {
        AppleBounce();
        return;
    }
    //first hit ground
    if(appleBounce == 0 && hitGround)
    {
        bounceTimer = Time::Get()->Running();
        appleBounce = 1;
        position.y += 4;
        bouncing = true;
    }
    else if (appleBounce == 1) {
        bounceTimer = Time::Get()->Running();
        appleBounce = 2;
        position.y += 4;
        bouncing = true;
    }
    else if (appleBounce == 2) {
        bounceTimer = Time::Get()->Running();
        appleBounce = 3;
        position.y += 8;
        bouncing = true;
    }
    else if (appleBounce == 3) {//roll
        if (__super::GetLeft()) {
            dir = Values::LeftVec;
        }
        else {
            dir = Values::RightVec;
        }
    }
    if (appleBounce == 0) {
        ChangeAnimation(this->clipname, velocity, dir, 0, true);
        return;
    }
    ChangeAnimation(this->clipname, velocity, dir, 0, false);
}

void Enemy::AppleBounce()
{
    int velocity = Time::Delta() * VELOCITY * 1.6f;
    Vector3 dir;
    float dur = bounceDuration[appleBounce - 1];
    if (Time::Get()->Running() - bounceTimer < (dur / 2.0f)) {
        dir = Values::UpVec;

    }
    else if (Time::Get()->Running() - bounceTimer < dur + 0.1f) {
        dir = Values::DownVec;
    }

    if (hitGround) {
        bouncing = false;
        return;
    }
    if (__super::GetLeft()) {
        dir += Values::LeftVec;
    }
    else {
        dir += Values::RightVec;
    }
    ChangeAnimation(this->clipname, velocity, dir, 0, false);
}

void Enemy::AttackPlayer()
{
}

void Enemy::rangedAttackPlayer()
{
}

void Enemy::SetDeathStart()
{
    sparkEffect.StartTimer(0.0f);
    beamEffect.StartTimer(0.0f);

    state = DEATH;
}

