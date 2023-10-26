#pragma once

#define BOSSPNGNUM 4

enum State {
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

    void Idle();
    void AppleAtk();
    void Attack();
    void SetHit();
    void SetDeath();


private:
    int health = 6;
    State state = idle;
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
};