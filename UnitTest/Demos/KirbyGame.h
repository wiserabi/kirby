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
    void CheckReset();
    void Sound();
    void UpdateEffect();
    void SetThrowStar();
    void SetKirbyEat();
    void SetKirbyBlowAir();
    bool KirbyCollisionWithWorld(BoundingBox* kirbyBox, Rect* worldRect);
    int CheckSlopeRange(float positionX);
    void SetKirbyPosForSlope(int idx, Vector3 kirbyPos, float rotation);
    void SetEnemyPosForSlope(int idx, Vector3 enemyPos, float rotation, class Enemy* enemy);
    void CheckInhaleEnemy(int kirbyLocation, vector<class Level*> levels);
    void CheckHitByEnemy(vector<Enemy*> enemies, int idx);
    void EnemyAttackCollideKirby(Rect* effect);
    //whether kirby is hit by enemy
    void EnemyAndKirby(int kirbyLocation, vector<class Level*> levels);//interactions between enemy and kirby
    void BossAndKirby(int kirbyLocation, vector<class Level*> levels);//interaction between boss and kirby
    void CheckBlowAirHitEnemy(vector<Enemy*> enemies, int idx);
    void CheckStarHitEnemy(vector<Enemy*> enemies, int idx);
    void CheckStarHitBoss(Rect* bossRect, int kirbyLocation,
        vector<class Level*> levels);

    void KillEnemyWithEffect(vector<Enemy*> enemies, int idx);
    BoundingBox* GetEnemyBox(vector<Enemy*> enemies, int idx);

private:
    class KirbyCharacter* kirby = nullptr;
    class AnimationRect* p1 = nullptr;
    class Sound* s = nullptr;
    class HUD* hud = nullptr;
    class World* world = nullptr;
    vector<class KirbyEffect*> effects;
    vector<pair<float, float>> slopeRange;
    float floor = 340.0f;
    float floor2 = 476.0f;
    float tan27 = 0.509525449f;
    bool prevTimerSet = false, curTimerSet = false;
    BoundingBox* effectBox = nullptr;
    Rect* effectRect = nullptr;
    BoundingBox* enemyBox = nullptr;
    Rect* enemyRect = nullptr;
    int kirbyLocation = 0;
    int deathCnt = 0;
    bool nothing = false;
    float invincibleDuration = 2.0f;
};
