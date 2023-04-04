#include <glfw3.h>

#include "glm/gtc/matrix_transform.hpp"

#include "../event/event.h"

#include "camera.h"

using namespace Engine;

constexpr glm::vec3 g_caemraPos = glm::vec3(0.0f, 0.0f, 1.0f);
constexpr glm::vec3 g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
constexpr glm::vec3 g_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

constexpr float g_cameraSpeedCoeff = 7.5f;

Camera::Camera(float width, float height)
	: m_pos(0.0f, 0.0f, 3.0f)
	, m_cameraFront(g_cameraFront)
	, m_firstMove(true)
	, m_lastX(width / 2.0f)
	, m_lastY(height / 2.0f)
	, m_yaw(-90.0f)
	, m_pitch(0.0f)
	, m_moveType(0)
{
	m_cameraView = glm::lookAt(
		m_pos,
		m_pos + m_cameraFront,
		glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::update(float dt)
{
	glm::vec3 velocity = glm::vec3(0.0f);

	if (m_moveType & (unsigned int)MoveType::Front)
	{
		velocity += m_cameraFront * g_cameraSpeedCoeff;
	}

	if (m_moveType & (unsigned int)MoveType::Back)
	{
		velocity -= m_cameraFront * g_cameraSpeedCoeff;
	}

	if (m_moveType & (unsigned int)MoveType::Left)
	{
		velocity -= glm::cross(m_cameraFront, g_cameraUp) * g_cameraSpeedCoeff;
	}

	if (m_moveType & (unsigned int)MoveType::Right)
	{
		velocity += glm::cross(m_cameraFront, g_cameraUp) * g_cameraSpeedCoeff;
	}

	m_pos += velocity * dt;

	m_cameraView = glm::lookAt(
		m_pos,
		m_pos + m_cameraFront,
		glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::handleInput(KeyboardEvent event)
{
	if (event.m_key == GLFW_KEY_W)
	{
		if (event.m_action == GLFW_PRESS || event.m_action == GLFW_REPEAT)
		{
			m_moveType = m_moveType | (unsigned int)MoveType::Front;
		}
		else
		{
			m_moveType = m_moveType & ~((unsigned int)MoveType::Front);
		}
	}

	if (event.m_key == GLFW_KEY_S)
	{
		if (event.m_action == GLFW_PRESS || event.m_action == GLFW_REPEAT)
		{
			m_moveType = m_moveType | (unsigned int)MoveType::Back;
		}
		else
		{
			m_moveType = m_moveType & ~((unsigned int)MoveType::Back);
		}
	}

	if (event.m_key == GLFW_KEY_A)
	{
		if (event.m_action == GLFW_PRESS || event.m_action == GLFW_REPEAT)
		{
			m_moveType = m_moveType | (unsigned int)MoveType::Left;
		}
		else
		{
			m_moveType = m_moveType & ~((unsigned int)MoveType::Left);
		}
	}

	if (event.m_key == GLFW_KEY_D)
	{
		if (event.m_action == GLFW_PRESS || event.m_action == GLFW_REPEAT)
		{
			m_moveType = m_moveType | (unsigned int)MoveType::Right;
		}
		else
		{
			m_moveType = m_moveType & ~((unsigned int)MoveType::Right);
		}
	}
}

void Camera::handleInput(MouseMoveEvent event)
{
	if (m_firstMove)
	{
		m_lastX = event.x;
		m_lastY = event.y;
		m_firstMove = false;
	}

	float xOffset = event.x - m_lastX;
	float yOffset = m_lastY - event.y;
	m_lastX = event.x;
	m_lastY = event.y;

	float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	m_yaw += xOffset;
	m_pitch += yOffset;

	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}
	if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}

	glm::vec3 direction;
	direction.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
	direction.y = glm::sin(glm::radians(m_pitch));
	direction.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
	m_cameraFront = glm::normalize(direction);
}
