#include <GLFW/glfw3.h>

#include "glm/gtc/matrix_transform.hpp"

#include "../event/event.h"

#include "camera.h"

using namespace Engine;

constexpr glm::vec3 g_caemraPos = glm::vec3(0.0f, 0.0f, 1.0f);
constexpr glm::vec3 g_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
constexpr glm::vec3 g_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

constexpr float g_cameraSpeedCoeff = 10.0f;

Camera::Camera(glm::vec3 pos, float width, float height)
	: m_pos(pos)
	, m_velocity(0.0f)
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
	if (m_moveType & (unsigned int)MoveType::Front)
	{
		m_velocity += m_cameraFront;
	}

	if (m_moveType & (unsigned int)MoveType::Back)
	{
		m_velocity -= m_cameraFront;
	}

	if (m_moveType & (unsigned int)MoveType::Left)
	{
		m_velocity -= glm::cross(m_cameraFront, g_cameraUp);
	}

	if (m_moveType & (unsigned int)MoveType::Right)
	{
		m_velocity += glm::cross(m_cameraFront, g_cameraUp);
	}

	if (m_moveType == (unsigned int)MoveType::None)
	{
		m_velocity = glm::vec3(0.0f);
	}
	else
	{
		m_velocity = g_cameraSpeedCoeff * glm::normalize(m_velocity);
	}


	m_pos += m_velocity * dt;

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

void Camera::handleInput(MouseMoveEvent event, float dt)
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

	float sensitivity = 10.0f;
	xOffset *= sensitivity * dt;
	yOffset *= sensitivity * dt;

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
