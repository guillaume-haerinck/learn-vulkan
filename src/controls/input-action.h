#pragma once

#include <glm/glm.hpp>

struct InputAction
{
	bool camOrbit = false;
	bool camPan = false;
	bool camDolly = false;
	float mouseWheelDelta = 0;
	glm::vec2 mousePos = glm::vec2(0);
	glm::vec2 mousePosDelta = glm::vec2(0);
};
