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

	//disable camera auto move
	if (key->Down(VK_F10)) {
		if (cameraAutoMove) {
			cameraAutoMove = false;
		}
		else {
			cameraAutoMove = true;
		}
	}


	//cout << cameraAutoMove << "\n";
	if (cameraAutoMove) {
		position.x = kirbyPos.x - WinMaxWidth / 2;
		position.y = kirbyPos.y - WinMaxHeight / 2;

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
	else {
		if (key->Press('1')) {
			position.y += 10;
		}
		else if (key->Press('2')) {
			position.y -= 10;
		}

		if (key->Press('3')) {
			position.x -= 10;
		}
		else if (key->Press('4')) {
			position.x += 10;
		}
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

