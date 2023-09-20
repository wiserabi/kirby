#include "Framework.h"
#include "Camera.h"

Camera::Camera()
{
	vpb = new VPBuffer();

	D3DXMatrixLookAtLH
	(
		&view,
		&Vector3(0, 0, 0),
		&Vector3(0, 0, 1),
		&Vector3(0, 1, 0)
	);

	D3DXMatrixOrthoOffCenterLH
	(
		&projection,
		0.0f,
		(float)WinMaxWidth,
		0.0f,
		(float)WinMaxHeight,
		0,
		1
	);

	vpb->SetView(view);
	vpb->SetProjection(projection);
}

Camera::~Camera()
{
	SAFE_DELETE(vpb);
}

void Camera::Update()
{
	Move();
	UpdateView();
}

void Camera::Render()
{
	vpb->SetVSBuffer(1);
}

void Camera::Move()
{
	auto key = Keyboard::Get();
	float delta = Time::Delta();

	if (key->Press(VK_RIGHT)) {
		if (WinMaxWidth / 2 < kirbyPos.x) {
			position.x += cameraSpeed * delta;
		}
	}
	if (key->Press(VK_LEFT)) {
		if (this->bottomRight.x + WinMaxWidth / 2 > kirbyPos.x) {
			position.x -= cameraSpeed * delta;
		}
	}
	if (key->Press(VK_UP)) {
		if (this->bottomRight.y < kirbyPos.y) {
			position.y += cameraSpeed * delta;
		}
	}
	if (key->Press(VK_DOWN)) {
		if (this->topLeft.y - WinMaxHeight / 2 > kirbyPos.y) {
			position.y -= cameraSpeed * delta;
		}
	}
	if (position.x < topLeft.x) {
		position.x = topLeft.x;
	}
	if (position.x > bottomRight.x) {
		position.x = bottomRight.x;
	}
	if (position.y < bottomRight.y) {
		position.y = bottomRight.y;
	}
	if (position.y > topLeft.y) {
		position.y = topLeft.y;
	}
}

void Camera::UpdateView()
{
	D3DXMatrixLookAtLH(&view, &position, &(position + Values::FwdVec), &Values::UpVec);
	vpb->SetView(view);
}

void Camera::SetBound(Vector3 topLeft, Vector3 bottomRight)
{
	this->topLeft = topLeft;
	this->bottomRight = bottomRight;
}

void Camera::SetKirbyPosition(Vector3 kirbyPos)
{
	this->kirbyPos = kirbyPos;
}

void Camera::Unprojection(Vector3* out, Vector3 source, Matrix world)
{
	Vector3 position = source;
	
	out->x = (position.x * 2.0f / WinMaxWidth) - 1.0f;
	out->y = (position.y * 2.0f / WinMaxHeight) - 1.0f;
	out->z = position.z;

	Matrix wvp = world * view * projection;
	D3DXMatrixInverse(&wvp, nullptr, &wvp);
	D3DXVec3TransformCoord(out, out, &wvp);
}

