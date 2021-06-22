#pragma once
namespace radx
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		void SetCamera(XMVECTOR position, XMVECTOR mRotate);
		void MoveCamera(float x, float y, float z);
		void MoveForward(float distance);
		void MoveBackward(float distance);
		void StrafeLeft(float distance);
		void StrafeRight(float distance);
		void RotateCamera(float pitch, float yaw, float Roll);
		XMMATRIX GetViewMatrix() const;

		const XMVECTOR& GetPosition();

	private:
		XMVECTOR mPosition;
		XMVECTOR mRotation;
	};
}