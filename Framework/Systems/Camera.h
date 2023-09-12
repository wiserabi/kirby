#pragma once


class Camera : public SingletonBase<Camera>
{
public:
	friend class SingletonBase<Camera>;

	void Update();
	void Render();

	void Move();
	void UpdateView();

	void Unprojection(Vector3* out, Vector3 source, Matrix world = Values::Identity);

private:
	Camera();
	~Camera();

private:
	VPBuffer* vpb = nullptr;
	Matrix view, projection;

	Vector3 position = Values::ZeroVec3;
	float cameraSpeed = 100.0f;
};