#include "Camera.h"

#include <Mouse.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera()
{
	//functional
	m_movespeed = 0.30;
	m_camRotRate = 3.0;

	//camera
	m_camPosition.x = 0.0f;
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;

	m_camLookAt.x = 0.0f;
	m_camLookAt.y = 0.0f;
	m_camLookAt.z = 0.0f;

	m_camLookDirection.x = 0.0f;
	m_camLookDirection.y = 0.0f;
	m_camLookDirection.z = 0.0f;

	m_camRight.x = 0.0f;
	m_camRight.y = 0.0f;
	m_camRight.z = 0.0f;

	m_camOrientation.x = 0.0f;
	m_camOrientation.y = 0.0f;
	m_camOrientation.z = 0.0f;

}

Camera::~Camera()
{
}

void Camera::Update(DirectX::Mouse::State m_mouseState, DirectX::Mouse::State m_mousePreviousState)
{
	HWND hwnd = GetActiveWindow();
	RECT windowRect;
	GetWindowRect(hwnd, &windowRect);

	// Get the client area center
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	POINT center;
	center.x = (clientRect.right - clientRect.left) / 2.f + clientRect.left;
	center.y = (clientRect.bottom - clientRect.top) / 2.f + clientRect.top;

	// Convert to screen coordinates
	POINT screenCenter = center;
	ClientToScreen(hwnd, &screenCenter);

	POINT point;
	GetCursorPos(&point);

	if(m_mouseState.rightButton && m_mousePreviousState.rightButton)
	{
		SetMouseVisible(false);

		if (m_mouseUpdated)
		{
			m_prevMouseX = m_mouseState.x;
			m_prevMouseY = m_mouseState.y;
			m_mouseUpdated = false;
		}

		// Calculate mouse delta from center
		int deltaX = point.x - screenCenter.x;
		int deltaY = point.y - screenCenter.y;

		m_prevMouseX = m_mouseState.x;
		m_prevMouseY = m_mouseState.y;

		float sensitivity = 0.1f;
		m_camOrientation.y += -deltaX * sensitivity;
		m_camOrientation.x += -deltaY * sensitivity;

		// Clamp pitch
		if (m_camOrientation.x > 89.0f) m_camOrientation.x = 89.0f;
		if (m_camOrientation.x < -89.0f) m_camOrientation.x = -89.0f;

		SetCursorPos(screenCenter.x, screenCenter.y);
	}
	if (m_mousePreviousState.rightButton == true && m_mouseState.rightButton == false)
	{
		SetMouseVisible(true);
	}



	//TODO  any more complex than this, and the camera should be abstracted out to somewhere else
	//camera motion is on a plane, so kill the 7 component of the look direction
	Vector3 planarMotionVector = m_camLookDirection;
	planarMotionVector.y = 0.0;

	if (m_InputCommands.rotRight)
	{
		m_camOrientation.y -= m_camRotRate;
	}
	if (m_InputCommands.rotLeft)
	{
		m_camOrientation.y += m_camRotRate;
	}

	//create look direction from Euler angles in m_camOrientation
	// Convert from degrees to radians
	float pitch = m_camOrientation.x * 3.1415f / 180.0f;
	float yaw = m_camOrientation.y * 3.1415f / 180.0f;

	m_camLookDirection.x = cosf(pitch) * sinf(yaw);
	m_camLookDirection.y = sinf(pitch);
	m_camLookDirection.z = cosf(pitch) * cosf(yaw);
	m_camLookDirection.Normalize();

	//create right vector from look Direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

	//process input and update stuff

	if (m_camInput.x > 0.1)
	{
		m_camPosition += m_camLookDirection * m_movespeed;
	}
	if (m_camInput.x < 0.0)
	{
		m_camPosition -= m_camLookDirection * m_movespeed;
	}
	if (m_camInput.z > 0.1)
	{
		m_camPosition += m_camRight * m_movespeed;
	}
	if (m_camInput.z < 0.0)
	{
		m_camPosition -= m_camRight * m_movespeed;
	}

	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;

	//apply camera vectors
	m_view = Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY);

	m_camInput = Vector3(0, 0, 0);
}

void Camera::SetInputVector(DirectX::SimpleMath::Vector3 IPVector)
{
	m_camInput += IPVector;
}

void Camera::SetMouseVisible(bool isVisible)
{
	if(isVisible)
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
	else
	{
		SetCursor(NULL);
	}
}
