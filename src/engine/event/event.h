#pragma once

namespace Engine
{
	enum class EventType
	{
		Keyboard,
		MouseMove,
		Close
	};

	struct Event
	{
		virtual inline EventType getType() const = 0;
	};

	struct KeyboardEvent : public Event
	{
		int m_key;
		int m_action;

		inline EventType getType() const override{ return EventType::Keyboard; }
	};

	struct MouseMoveEvent : public Event
	{
		int x;
		int y;

		inline EventType getType() const override { return EventType::MouseMove; }
	};

	struct CloseEvent : public Event
	{
		inline EventType getType() const override { return EventType::Close; }
	};

}
