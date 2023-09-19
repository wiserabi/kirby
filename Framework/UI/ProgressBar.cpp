#include "Framework.h"
#include "ProgressBar.h"

ProgressBar::ProgressBar(Vector3 position, Vector3 size, float rotation, Color color, FillType type)
	: type(type)
{
	Init(position, size, rotation, color, type);
}

ProgressBar::ProgressBar(Vector3 position, Vector3 size, float rotation, wstring path, FillType type)
	: type(type)
{
	Init(position, size, rotation, path, type);
	pb = new ProgressBuffer(type);
}

ProgressBar::ProgressBar(Vector3 position, Vector3 size, float rotation, FillType type)
	: type(type)
{
	Init(position, size, rotation, type);
	pb = new ProgressBuffer(type);
}

ProgressBar::~ProgressBar()
{
	SAFE_DELETE(pb);
}

void ProgressBar::Update()
{
	if (pb)
		pb->UpdateProgressPercent(percent);

	UI::Update();
}

void ProgressBar::Render()
{
	if (pb)
		pb->SetPSBuffer(2);

	UI::Render();
}

void ProgressBar::UpdateProgressBar(float percent)
{
	this->percent = percent;
	cout << String::ToString(size) << endl;

	if (pb)
		return;

	Vector3 changeSize = Values::ZeroVec3;

	switch (type)
	{
	case UI::NONE:
	{
		changeSize = Vector3(originSize.x * percent, originSize.y * percent, 0.0f);
		break;
	}

	case UI::LEFT_TO_RIGHT:
	case UI::RIGHT_TO_LEFT:
	{
		changeSize = Vector3(originSize.x * percent, originSize.y, 0.0f);
		break;
	}

	case UI::UP_TO_DOWN:
	case UI::DOWN_TO_UP:
	{
		changeSize = Vector3(originSize.x, originSize.y * percent, 0.0f);
		cout << String::ToString(changeSize) << endl;
		break;
	}
	}
	size = changeSize;
}