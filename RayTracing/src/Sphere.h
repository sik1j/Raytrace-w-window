#pragma once
#include "glm/glm.hpp"

class Sphere
{
public:
	glm::vec3 coords = glm::vec3(0.0f,0.0f,0.0f);

	float coordsArr[3] = {0.0f, 0.0f, 0.0f};

	glm::vec4 spehreColor = glm::vec4(1.0f,1.0f,0.0f,1.0f);
};
