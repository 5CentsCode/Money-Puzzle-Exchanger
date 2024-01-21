#pragma once
#include "PCH.h"
#include <glm/glm.hpp>

class Sprite
{
public:

	glm::ivec2 Position;
	glm::ivec2 Size;
	uint32 TextureId;

public:

	glm::mat4 GetWorldMatrix();
};

