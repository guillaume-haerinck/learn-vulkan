#include "arcball-camera.h"
#include "arcball-camera.h"

#include <glm/gtc/matrix_access.hpp>

ArcballCamera::ArcballCamera()
	: m_theta(1), m_phi(1), m_up(1), m_radius(10), m_position(8, 5, 7), m_target(0, 0, 0)
{
	m_view = glm::lookAtLH(m_position, m_target, glm::vec3(0, m_up, 0));
	m_proj = glm::perspectiveFovLH(glm::quarter_pi<float>(), 800.0f, 600.0f, 0.1f, 100.0f);
	m_viewProj = m_proj * m_view;
}

ArcballCamera::~ArcballCamera()
{
}

void ArcballCamera::update(const InputAction& inputs)
{
	bool camHasToBeUpdated = false;

	if (inputs.camOrbit) {
		m_theta -= inputs.mousePosDelta.x * 0.01f;
		m_phi += inputs.mousePosDelta.y * 0.01f;

		// Keep phi within -2PI to +2PI for easy 'up' comparison
		if (m_phi > glm::two_pi<float>())
			m_phi -= glm::two_pi<float>();
		else if (m_phi < -glm::two_pi<float>())
			m_phi += glm::two_pi<float>();

		// If phi is between 0 to PI or -PI to -2PI, make 'up' be positive Y, other wise make it negative Y
		if ((m_phi > 0 && m_phi < glm::pi<float>()) || (m_phi < -glm::pi<float>() && m_phi > -glm::two_pi<float>()))
			m_up = 1.0f;
		else
			m_up = -1.0f;

		camHasToBeUpdated = true;
	}

	if (inputs.camDolly) {
		if (inputs.mouseWheelDelta > 0 && m_radius >= 2)
			m_radius -= 1.5f;
		else if (inputs.mouseWheelDelta < 0)
			m_radius += 1.5f;

		camHasToBeUpdated = true;
	}

	if (inputs.camPan) {
		glm::mat4x4 invView = glm::inverse(m_view);
		glm::vec4 col0 = glm::normalize(glm::column<glm::mat4x4>(invView, 0));
		glm::vec4 col1 = glm::normalize(glm::column<glm::mat4x4>(invView, 1));
		glm::vec4 movement = col0 * inputs.mousePosDelta.x + col1 * -inputs.mousePosDelta.y;

		m_target += glm::vec3(movement.x, movement.y, movement.z) * 0.04f;

		camHasToBeUpdated = true;
	}

	if (camHasToBeUpdated) {
		m_position.x = m_target.x + m_radius * sinf(m_phi) * sinf(m_theta);
		m_position.y = m_target.y + m_radius * cosf(m_phi);
		m_position.z = m_target.z + m_radius * sinf(m_phi) * cosf(m_theta);

		glm::vec3 eye = m_position;
		glm::vec3 target = m_target;
		glm::vec3 up = glm::vec3(0, m_up, 0);

		m_view = glm::lookAtLH(eye, target, up);
		m_viewProj = m_proj * m_view;
	}
}

const glm::mat4x4& ArcballCamera::getViewProj()
{
	return m_viewProj;
}
