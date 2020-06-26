#pragma once

#include <glm/glm.hpp>

struct InputAction
{
	bool camOrbit = false;
	glm::vec2 mousePos = glm::vec2(0);
	glm::vec2 mousePosDelta = glm::vec2(0);
};
