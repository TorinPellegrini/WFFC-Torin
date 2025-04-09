#pragma once
#include <GamePad.h>

#include <memory>
#include <d3d11.h>
#include <Mouse.h>
#include <SimpleMath.h>

#include "InputCommands.h"

class Camera
{
public:
	Camera();
	~Camera();

	void Update(DirectX::Mouse::State m_mouseState, DirectX::Mouse::State m_mousePreviousState);

	void SetInputVector(DirectX::SimpleMath::Vector3 IPVector);
	void RotateCamera(bool isRight);

	InputCommands						m_InputCommands;

	DirectX::SimpleMath::Vector3		m_camPosition;

	DirectX::SimpleMath::Matrix	m_view;


private:

	DirectX::SimpleMath::Vector3 m_camInput;

	//functionality
	float								m_movespeed;

	//camera
	DirectX::SimpleMath::Vector3		m_camOrientation;
	DirectX::SimpleMath::Vector3		m_camLookAt;
	DirectX::SimpleMath::Vector3		m_camLookDirection;
	DirectX::SimpleMath::Vector3		m_camRight;
	float m_camRotRate;


	int m_prevMouseX = 0;
	int m_prevMouseY = 0;
	bool m_mouseUpdated = true;

	void SetMouseVisible(bool isVisible);

};

