#include "pch.h"
#include "Camera.h"

namespace radx
{
	Camera::Camera()
		: mPosition(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f))
		, mRotation(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f))
	{
	}


	Camera::~Camera()
	{
	}

	void Camera::SetCamera(XMVECTOR position, XMVECTOR rotation)
	{
		mPosition = position;
		mRotation = rotation;
	}

	void Camera::MoveCamera(float x, float y, float z)
	{
		mPosition.m128_f32[0] += x;
		mPosition.m128_f32[1] += y;
		mPosition.m128_f32[2] += z;
	}

	void Camera::MoveForward(float distance)
	{
		XMVECTOR toward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		toward = XMVector3Transform(toward, XMMatrixRotationRollPitchYawFromVector(mRotation));
		toward *= distance;
		mPosition += toward;
	}

	void Camera::MoveBackward(float distance)
	{
		XMVECTOR toward = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		toward = XMVector3Transform(toward, XMMatrixRotationRollPitchYawFromVector(mRotation));
		toward *= distance;
		mPosition += toward;
	}

	void Camera::StrafeLeft(float distance)
	{
		XMVECTOR toward = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
		toward = XMVector3Transform(toward, XMMatrixRotationRollPitchYawFromVector(mRotation));
		toward *= distance;
		mPosition += toward;
	}

	void Camera::StrafeRight(float distance)
	{
		XMVECTOR toward = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		toward = XMVector3Transform(toward, XMMatrixRotationRollPitchYawFromVector(mRotation));
		toward *= distance;
		mPosition += toward;
	}

	void Camera::RotateCamera(float pitch, float yaw, float Roll)
	{
		mRotation.m128_f32[0] += pitch;
		mRotation.m128_f32[1] += yaw;
		mRotation.m128_f32[2] += Roll;
	}

	XMMATRIX Camera::GetViewMatrix() const
	{
		float pitch = mRotation.m128_f32[0];
		float yaw = mRotation.m128_f32[1];
		float roll = mRotation.m128_f32[2];

		XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
		lookAt = XMVector3Transform(lookAt, rotationMatrix);
		up = XMVector3Transform(up, rotationMatrix);

		lookAt = mPosition + lookAt;

		return XMMatrixLookAtLH(mPosition, lookAt, up);
	}

	const XMVECTOR& Camera::GetPosition()
	{
		return mPosition;
	}

}