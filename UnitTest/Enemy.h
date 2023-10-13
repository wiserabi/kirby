#pragma once

class Enemy : public AnimationRect{
public:
    Enemy(Vector3 position, Vector3 size, string name, class EnemyInfo* infos);
    ~Enemy();

    virtual void Update();
    virtual void Render();

    virtual void Move();

    virtual void SetAnimator(class Animator* animator);
    virtual void SetPosition(Vector3 pos);
    
    void SetEnemyInfo(class EnemyInfo* infos);
    void Walk();
    void Jump();
    void Fly();
    void MoveIdle();
    void AttackPlayer();
    void rangedAttackPlayer();
private:
    enum State { IDLE, CHASE, ATTACK };
    State state = IDLE;
    enum Movement { STOP, WALKRIGHT, WALKLEFT};
    Movement move = STOP;
    std::vector<std::vector<int>> Enemy::currentMap;
    Vector3 kirbyPos;

    float inRange = 400.0f;
    float outRange = 450.0f;
    float attackRange = 100.0f;
    float idleTimer = 0.0f;
    EnemyInfo* infos = nullptr;
    class Rect* rect = nullptr;
    wstring clipname;

    void setKirbyPos(Vector3 pos);
    bool playerInRange(float range){ 
        return sqrt(pow(kirbyPos.x - position.x, 2.0f) + pow(kirbyPos.y - position.y, 2.0f)) < range;
    }
    bool playerOutOfRange(float range) {
        return sqrt(pow(kirbyPos.x - position.x, 2.0f) + pow(kirbyPos.y - position.y, 2.0f)) > range;
    }
    bool canAttack(float range) {
        return sqrt(pow(kirbyPos.x - position.x, 2.0f) + pow(kirbyPos.y - position.y, 2.0f)) > range;
    }

    void moveTowardsPlayer();

    void attackPlayer();
};