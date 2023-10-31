#pragma once
class Ending : public IObject
{
public:
    virtual void Init() override;
    virtual void Destroy() override;
    virtual void Update() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUI() override;
private:
    vector<class TextureRect*> endingAnimation;
    class TextureRect* endingCredit;
    wstring path;
    Vector3 position = { WinMaxWidth / 2.0f, WinMaxHeight / 2.0f, 0.0f };
    Vector3 size = { WinMaxWidth, WinMaxHeight, 0.0f };
    size_t frames = 293;
    size_t curFrame = 0;
    size_t speed = 0;
    bool end = false;
};