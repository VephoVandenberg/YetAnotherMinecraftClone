#include "../engine/event/event.h"

#include "player.h"

using namespace GameNamespace;

Player::Player()
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
	}
}

void Player::update(float dt)
{
	m_camera.update(dt);
}
