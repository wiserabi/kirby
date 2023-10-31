#pragma once

class Program : public IObject
{
	virtual void Init() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUI() override;

private:
	void Push(IObject* obj);

	Matrix view, proj;
	VPBuffer* vpb = nullptr;

	vector<IObject*> objs;
	int curObj = 0;
	bool showEnding = false;
};