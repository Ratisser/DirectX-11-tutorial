#pragma once
class Camera
{
public:
	Camera();
	~Camera();

	void SetCamera(XMVECTOR position, XMVECTOR mRotate);
	void MoveCamera(float x, float y, float z);
	void RotateCamera(float pitch, float yaw, float Roll);
	XMMATRIX GetViewMatrix() const;

private:
	XMVECTOR mPosition;
	XMVECTOR mRotation;
};

