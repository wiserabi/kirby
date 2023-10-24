#pragma once

class KirbyGame : public IObject
{
public:
    virtual void Init() override;
    virtual void Destroy() override;
    virtual void Update() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUI() override;
    void SetCameraBound();
    void FixKirbyPosition(class Rect* worldRect);
    void FixEnemyPosition(class Rect* worldRect, int idx);
    void CheckReset();
    void Sound();
    void UpdateEffect();
    void CheckInhaleEnemy();
    void SetThrowStar();
    void SetKirbyEat();
    void SetKirbyBlowAir();
    bool KirbyCollisionWithWorld(BoundingBox* kirbyBox, Rect* worldRect);
    void EnemyCollisions(vector<class Enemy*>& enemies, Rect* worldRect, BoundingBox* kirbyBox);
    void EnemyAttackCollideKirby(class Rect* effect);//where to move kirby when collide with effect
    void CheckAbility();
    int CheckSlopeRange();
    void SetKirbyPosForSlope(int idx, Vector3 kirbyPos, float rotation);

private:
    class KirbyCharacter* kirby = nullptr;
    class AnimationRect* p1 = nullptr;
    class Sound* s = nullptr;
    class HUD* hud = nullptr;
    class World* world = nullptr;
    class EnemyInfo* enemyInfo = nullptr;
    vector<class Enemy*> enemies;
    vector<class KirbyEffect*> effects;
    vector<pair<class Enemy*, int>> enemySwallowed;
    vector<pair<float, float>> slopeRange;
    float floor = 340.0f;
    float floor2 = 476.0f;
    float tan27 = 0.509525449f;
};
