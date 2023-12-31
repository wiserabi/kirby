#pragma once

#define BOSSPNGNUM 4

enum BossState {
    idle,
    atk,
    appleatk,
    hit,
    death
};

class TreeBoss :public AnimationRect{
public:
	TreeBoss(Vector3 position, Vector3 size);
	~TreeBoss();

    virtual void Update();
    virtual void Render();

    virtual void Move();
    virtual void SetAnimator(class Animator* animator);

    void AppleAtk();//throw apple
    void Attack();//blow wind

    int GetState();//get state of boss
    void SetState(int state);//set state of boss
    void SaveState();//save current state of boss
    void SetHitTimer();//set time when boss got hit
    int GetPrevState();
    class Rect* GetRect();
    void SetTimer();
    int GetHealth() { return health; }
    void SetHealth(int health);
    class Rect* GetEndingRect() { return endingRect; }

private:
    int health = 28;
    BossState state = idle;
    BossState prevState = idle;
    BossState saveState = idle;
    
    float timer = 0.0f;
    float hitTimer = 0.0f;
    bool ending = false;
    float endingPosX = 21150.0f;
    float endingPosY = -10.0f;
    Vector3 endingPos;
    Rect* endingRect = nullptr;


    wstring png[BOSSPNGNUM] = {
        L"treebossidle.png",
        L"treebossatk.png",
        L"treebossdefeat.png",
        L"treebosshit.png",
    };
    int split[BOSSPNGNUM] = {
        4,3,2,2
    };
    class Rect* rect = nullptr;
    wstring current = L"treebossidle.png";
    float hitPositionY;
    float defaultPos;
    float idleTime = 4.0f;
    class AnimationRect* endingItem = nullptr;
};