#pragma once

class AnimationClip
{
public:
	friend class Animator;
	void SetPlayRate(float playRate) { this->playRate = playRate; }
	AnimationClip(wstring clipName, Texture2D* srcTex, 
		uint frameCount, Vector2 startPos, Vector2 endPos, 
		bool bReversed = false, float playRate = 1.0f/10.0f);
protected:
	wstring clipName = L"";
	vector<Vector2> keyFrames;
	uint frameCount = 0;
	ID3D11ShaderResourceView* srv = nullptr;
	Vector2 texelFrameSize = Values::ZeroVec2;
	float playRate = 0.0f;
	bool bReversed = false;
};

class Animator {
public:
	Animator();
	~Animator();

	void Update();

	Vector2 GetCurrentFrame() { return currentFrame; }
	uint GetCurrentFrameIndex() { return currentFrameIndex; }
	Vector2 GetTexelFrameSize() { return currentClip->texelFrameSize; }
	ID3D11ShaderResourceView* GetCurrentSRV() { return currentClip->srv; }

	void AddAnimClip(AnimationClip* animClip);
	void SetCurrentFrame(uint currentFrameIndex);
	void SetCurrentAnimClip(wstring clipName);
	void SetPlayRate(wstring clipName, float playRate);
	void SetFixAnimation(bool fixAnimation);

private:
	bool CheckExist(wstring clipName) { return animClips.find(clipName) != animClips.end(); }

private:
	unordered_map<wstring, AnimationClip*> animClips;

	AnimationClip* currentClip = nullptr;
	uint currentFrameIndex = 0;
	Vector2 currentFrame = Values::ZeroVec2;
	float deltaTime = 0.0f;
	bool fixAnimation = false;
};