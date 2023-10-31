#pragma once


class Camera : public SingletonBase<Camera>
{
public:
	friend class SingletonBase<Camera>;

	void Update();
	void Render();

	void Move();
	void UpdateView();
	void SetBound(Vector3 topLeft, Vector3 bottomRight);
	void SetKirbyPosition(Vector3 kirbyPos);

	void Unprojection(Vector3* out, Vector3 source, Matrix world = Values::Identity);
	const Vector3 GetPosition() { return position; }
	void SetAutoMove();
	void SetPosition(Vector3 position);

private:
	Camera();
	~Camera();

private:
	VPBuffer* vpb = nullptr;
	Matrix view, projection;

	Vector3 position = Values::ZeroVec3;
	float cameraSpeed = 100.0f;
	Vector3 topLeft = Values::ZeroVec3;
	Vector3 bottomRight = Values::ZeroVec3;
	Vector3 kirbyPos = Values::ZeroVec3;
	bool cameraAutoMove = true;
};