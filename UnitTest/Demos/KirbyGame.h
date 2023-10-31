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
        vector<class Level*> levels);//check if kirby star hit boss
    void CheckAbilityHitBoss(Rect* bossRect, int kirbyLocation, 
        vector<class Level*> levels);//check if kirby ability hit boss

    void KillEnemyWithEffect(vector<Enemy*> enemies, int idx);
    BoundingBox* GetEnemyBox(vector<Enemy*> enemies, int idx);
    void AddSounds();
    bool GetShowEnding() { return showEnding; }

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
    int kirbyPrevLocation = -1;
    int deathCnt = 0;//enemy death count
    int prevDeathCnt = 0;
    bool nothing = false;
    float invincibleDuration = 2.0f;
    bool startboss = false;
    int kirbyCurState;
    int kirbyPrevState;
    //sound durations
    float jumpSoundTime = 0.0f;
    float swallowSoundTime = 0.0f;
    float throwStarSoundTime = 0.0f;
    float sandwichedSoundTime = 0.0f;
    float headdownSoundTime = 0.0f;
    float hitSoundTime = 0.0f;
    float flyupSoundTime = 0.0f;
    float exhaleSoundTime = 0.0f;
    float getAbilitySoundTime = 0.0f;
    float enemykillSoundTime = 0.0f;
    float endDashSoundTime = 0.0f;
   
    float volume = 0.01f;
    bool prevAbilityUse = false;//kirby ability use previous
    bool abilityUse = false;//kirby ability use

    float bossInvulnerableTime = 1.6f;//boss invulnerable for 1.6f
    float bossHitTime = 0.0f;
    int dmg = 5;
    bool showEnding = false;

    vector<string> sounds = {
        "Vegetable-Valley.mp3",
        "29-Boss.mp3",
        "15-Forest Trail ~Intro~.mp3",
        "Level-Clear-Dance.mp3",
        "jump.mp3",
        "inhaling.mp3",
        "hitwall.mp3",
        "hit.mp3",
        "floatup.mp3",
        "exhale.mp3",
        "dashEnd.mp3",
        "squash.mp3",
        "getAbility.mp3",
        "throwStar.mp3",
        "enemykill.mp3",
        "spark.mp3",
        "beam.mp3",
    };
};
