#include "PCH.h"
#include "Sprite.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/matrix_decompose.hpp>

glm::mat4 Sprite::GetWorldMatrix()
{
	glm::mat4 worldMatrix = glm::identity<glm::mat4>();
	worldMatrix = glm::translate(worldMatrix, glm::vec3(Position, 0.0f));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(Size, 0.0f));
	return worldMatrix;
}