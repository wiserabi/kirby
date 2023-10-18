#pragma once

#include "KirbyEffect.h"

class Enemy : public AnimationRect{
public:
    Enemy(Vector3 position, Vector3 size, string name, class EnemyInfo* infos);
    ~Enemy();

    virtual void Update();
    virtual void Render();

    virtual void Move();

    virtual void SetAnimator(class Animator* animator);
    virtual void SetPosition(Vector3 pos);
    
    class Rect* GetRect();
    void SetEnemyInfo(class EnemyInfo* infos);
    void Walk();
    void Jump();
    void Fly();
    void MoveIdle();
    void AttackPlayer();
    void rangedAttackPlayer();
    int GetState() { return state; }
    bool CheckDeath();//check whether enemy is death

    void SetHitGround(bool hitGround) { this->hitGround = hitGround; }
    void SetHitLeft(bool hitLeft) { this->hitLeft = hitLeft; }
    void SetHitRight(bool hitRight) { this->hitRight = hitRight; }
    void SetDeathStart();
    void SetKirbyPos(Vector3 pos);


private:
    enum State { IDLE, CHASE, ATTACK, DEATH};
    State state = IDLE;
    enum Movement { STOP, WALKRIGHT, WALKLEFT};
    Movement move = WALKLEFT;
    std::vector<std::vector<int>> Enemy::currentMap;
    Vector3 kirbyPos;

    float inRange = 300.0f;
    float outRange = 400.0f;
    float attackRange = 100.0f;
    float idleTimer = 0.0f;
    EnemyInfo* infos = nullptr;
    class Rect* rect = nullptr;
    wstring clipname;

    bool hitGround = false;
    bool hitLeft = false;
    bool hitRight = false;
    class KirbyEffect deathEffect;
    class KirbyEffect attackEffect;
    float deathStart;
    float startAttack;

    bool playerOutOfRange(float range) {
        return sqrt(pow(kirbyPos.x - position.x, 2.0f) + pow(kirbyPos.y - position.y, 2.0f)) > range;
    }
    bool playerInRange(float range){ 
        return sqrt(pow(kirbyPos.x - position.x, 2.0f) + pow(kirbyPos.y - position.y, 2.0f)) < range;
    }
    bool canAttack(float range) {
        return hitGround && sqrt(pow(kirbyPos.x - position.x, 2.0f) + pow(kirbyPos.y - position.y, 2.0f)) < range;
    }

    void moveTowardsPlayer();

    void attackPlayer();
};