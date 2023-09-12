#pragma once

class AnimationDemo : public IObject
{
public:
    virtual void Init() override;
    virtual void Destroy() override;
    virtual void Update() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUI() override;

private:
    class AnimationRect* p1 = nullptr;
    class Sound* s = nullptr;
};
