#include <glfw3.h>

#include "glm/gtc/matrix_transform.hpp"

#include "../event/event.h"

#include "camera.h"

using namespace Engine;

constexpr glm::vec3 g_caemraPos = glm::vec3(0.0f, 0.0f, 1.0f);
constexpr glm::vec3 g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
constexpr glm::vec3 g_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

constexpr float g_cameraSpeedCoeff = 2.0f;

Camera::Camera()
	: m_pos(0.0f, 0.0f, 3.0f)
	, m_velocity(0.0f)
{
	m_cameraView = glm::lookAt(
		m_pos,
		m_pos + g_cameraFront,
		glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::update(float dt)
{
	m_pos += m_velocity * dt;

	m_cameraView = glm::lookAt(
		m_pos,
		m_pos + g_cameraFront,
		glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::handleInput(KeyboardEvent event)
{
	if (event.m_action == GLFW_PRESS || event.m_action == GLFW_REPEAT && glm::length(m_velocity) < 4)
	{
		switch (event.m_key)
		{
		case GLFW_KEY_W:
		{
			m_velocity += g_cameraSpeedCoeff * g_cameraFront;
		}break;

		case GLFW_KEY_S:
		{
			m_velocity += -g_cameraSpeedCoeff * g_cameraFront;
		}break;

		case GLFW_KEY_A:
		{
			m_velocity += -glm::normalize(glm::cross(g_cameraFront, g_cameraUp)) * g_cameraSpeedCoeff;
		}break;

		case GLFW_KEY_D:
		{
			m_velocity += glm::normalize(glm::cross(g_cameraFront, g_cameraUp)) * g_cameraSpeedCoeff;
		}break;
		
		default:
			break;
		}
	}
}
