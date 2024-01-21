#pragma once
#include <glm/gtc/matrix_transform.hpp>

struct Camera
{
	enum class ProjectionMode : uint8
	{
		Orthographic = 0,
		Perspective = 1
	};

public:

	Camera();
	~Camera();

	float GetNearClippingPlane() const;
	float GetFarClippingPlane() const;
	float GetFeildOfView() const;
	float GetAspectRatio() const;
	ProjectionMode GetProjectionMode() const;
	float GetOrthographicSize() const;

	void SetNearClippingPlane(float nearClippingPlane);
	void SetFarClippingPlane(float farClippingPlane);
	void SetFieldOfView(float fieldOfView);
	void SetAspectRatio(float aspectRatio);
	void SetProjectionMode(ProjectionMode projectionMode);
	void SetOrthographicSize(float orthographicSize);

	glm::mat4 GetProjectionMatrix() const;

private:

	void CalculateProjectionMatrix();

private:

	float m_nearClippingPlane = 0.1f;
	float m_farClippingPlane = 1000.0f;

	float m_fieldOfView = 45.0f;
	float m_aspectRatio = 16.0f / 9.0f;
	float m_orthographicSize = 5.0f;

	ProjectionMode m_projectionMode = ProjectionMode::Perspective;

	glm::mat4 m_projectionMatrix = glm::identity<glm::mat4>();
};