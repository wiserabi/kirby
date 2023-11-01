#pragma once
class Intro : public IObject
{
public:
    virtual void Init() override;
    virtual void Destroy() override;
    virtual void Update() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUI() override;
private:
    class AnimationRect* animation;
    Vector3 position = { WinMaxWidth / 2.0f, WinMaxHeight / 2.0f, 0.0f };
    Vector3 size = { 500.0f, 500.0f, 0.0f };
    int idx = 0;
};