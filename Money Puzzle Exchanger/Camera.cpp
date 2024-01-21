#include "PCH.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
	CalculateProjectionMatrix();
}

Camera::~Camera()
{
}

float Camera::GetNearClippingPlane() const
{
	return m_nearClippingPlane;
}

float Camera::GetFarClippingPlane() const
{
	return m_farClippingPlane;
}

float Camera::GetFeildOfView() const
{
	return m_fieldOfView;
}

float Camera::GetAspectRatio() const
{
	return m_aspectRatio;
}

Camera::ProjectionMode Camera::GetProjectionMode() const
{
	return m_projectionMode;
}

float Camera::GetOrthographicSize() const
{
	return m_orthographicSize;
}

void Camera::SetNearClippingPlane(float nearClippingPlane)
{
	m_nearClippingPlane = nearClippingPlane;
	CalculateProjectionMatrix();
}

void Camera::SetFarClippingPlane(float farClippingPlane)
{
	m_farClippingPlane = farClippingPlane;
	CalculateProjectionMatrix();
}

void Camera::SetFieldOfView(float fieldOfView)
{
	m_fieldOfView = fieldOfView;
	CalculateProjectionMatrix();
}

void Camera::SetAspectRatio(float aspectRatio)
{
	m_aspectRatio = aspectRatio;
	CalculateProjectionMatrix();
}

void Camera::SetProjectionMode(ProjectionMode projectionMode)
{
	m_projectionMode = projectionMode;
	CalculateProjectionMatrix();
}

void Camera::SetOrthographicSize(float orthographicSize)
{
	m_orthographicSize = orthographicSize;
	CalculateProjectionMatrix();
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

void Camera::CalculateProjectionMatrix()
{
	switch (m_projectionMode)
	{
		case ProjectionMode::Orthographic:
		{
			float halfWidth = m_aspectRatio * m_orthographicSize * 0.5f;
			float halfHeight = m_orthographicSize * 0.5f;
			m_projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, m_nearClippingPlane, m_farClippingPlane);
			break;
		}

		case ProjectionMode::Perspective:
		{
			m_projectionMatrix = glm::perspective(glm::radians(m_fieldOfView), m_aspectRatio, m_nearClippingPlane, m_farClippingPlane);
			break;
		}
	}

	m_projectionMatrix = glm::scale(m_projectionMatrix, glm::vec3(-1.0f, 1.0f, 1.0f));
}