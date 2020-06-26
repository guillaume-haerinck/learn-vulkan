#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "input-action.h"

class ArcballCamera {
public:
	ArcballCamera();
	~ArcballCamera();

	void update(const InputAction& inputs);
	const glm::mat4x4& getViewProj();

private:
	float m_theta;
	float m_phi;
	float m_up;
	float m_radius;
	glm::vec3 m_position;
	glm::vec3 m_target;
	glm::mat4x4 m_view;
	glm::mat4x4 m_proj;
	glm::mat4x4 m_viewProj;
};

