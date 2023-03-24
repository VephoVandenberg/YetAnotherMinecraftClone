#include "../engine/event/event.h"

#include "player.h"

using namespace GameNamespace;

Player::Player(float width, float height)
	: m_camera(width, height)
{

}

void Player::handleInput(Event& event)
{

	switch (event.m_type)
	{
	case EventType::Keyboard:
	{
		auto keyEvent = dynamic_cast<KeyboardEvent&>(event);
		m_camera.handleInput(keyEvent);
	}break;

	case EventType::MouseMove:
	{
		auto mouseMove = dynamic_cast<MouseMoveEvent&>(event);
		m_camera.handleInput(mouseMove);
	}
	}
}

void Player::update(float dt)
{
	m_camera.update(dt);
}
