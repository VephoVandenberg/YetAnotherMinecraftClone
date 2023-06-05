#include <GLFW/glfw3.h>
#include <iostream>

#include "../../engine/event/event.h"

#include "player.h"

using namespace Engine;
using namespace GameNamespace;

Player::Player(glm::vec3 pos, float width, float height)
	: m_camera(pos, width, height)
	, m_leftButtonClicked(false)
{}

void Player::handleInput(Event& event, float dt)
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
		m_camera.handleInput(mouseMove, dt);
	} break;

	case EventType::MouseClick:
	{
		auto& mouseClick = dynamic_cast<MouseClickEvent&>(event);
		// Should be changed because it is now adjusted for the block removal
		// And it should not be like that here

		if (mouseClick.m_button == GLFW_MOUSE_BUTTON_LEFT &&
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
