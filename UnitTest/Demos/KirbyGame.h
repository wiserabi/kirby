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


private:
    class KirbyCharacter* kirby = nullptr;
    class AnimationRect* p1 = nullptr;
    class Sound* s = nullptr;
    class HUD* hud = nullptr;
    class World* world = nullptr;
    class EnemyInfo* enemyInfo = nullptr;
    map<int, class Enemy*> enemies;
    //effect0 kirby inhaling and attack, effect1 kirby eat enemy
    vector<class KirbyEffect*> effects;
    //this is because there can be two effects at the same time

};
