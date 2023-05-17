#include <glfw3.h>

#include "../../engine/event/event.h"

#include "player.h"

using namespace Engine;
using namespace GameNamespace;

Player::Player(glm::vec3 pos, float width, float height)
	: m_camera(pos, width, height)
{}

void Player::handleInput(Event& event)
{
	switch (event.getType())
	{
	case EventType::Keyboard:
	{
		auto& keyEvent = dynamic_cast<KeyboardEvent&>(event);
		m_camera.handleInput(keyEvent);
	}break;

	case EventType::MouseMove:
	{
		auto& mouseMove = dynamic_cast<MouseMoveEvent&>(event);
		m_camera.handleInput(mouseMove);
	} break;

	case EventType::MouseClick:
	{
		auto& mouseClick = dynamic_cast<MouseClickEvent&>(event);

		if (mouseClick.m_button == GLFW_MOUSE_BUTTON_1 && 
			mouseClick.m_action == GLFW_PRESS)
		{
			m_leftButtonClicked = true;
			break;
		}
		m_leftButtonClicked = false;
	}
	}
}

void Player::update(float dt)
{
	m_camera.update(dt);
}
