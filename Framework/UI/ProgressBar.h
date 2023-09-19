#pragma once

#include "Utilities/UI.h"

class ProgressBuffer : public ShaderBuffer
{
public:
	struct Data
	{
		float percent;
		uint type;
		Vector2 dummy;
	};

public:
	ProgressBuffer(uint type) : ShaderBuffer(&data, sizeof(data))
	{
		data.percent = 1.0f;
		data.type = type;
	}

	void UpdateProgressPercent(float percent)
	{
		data.percent = percent;
	}

private:
	Data data;
};

class ProgressBar : public UI
{
public:
	ProgressBar(Vector3 position, Vector3 size, float rotation, Color color, FillType type = NONE);
	ProgressBar(Vector3 position, Vector3 size, float rotation, wstring path, FillType type = NONE);
	ProgressBar(Vector3 position, Vector3 size, float rotation, FillType type = NONE);
	virtual ~ProgressBar() override;

	virtual void Update() override;
	virtual void Render() override;

	void UpdateProgressBar(float percent);

protected:
	float percent = 1.0f;

private:
	FillType type;
	ProgressBuffer* pb = nullptr;
};