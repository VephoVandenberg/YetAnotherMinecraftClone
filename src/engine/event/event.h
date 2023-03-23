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
		EventType m_type;

		Event() = default;
		virtual ~Event() = default;
	};

	struct KeyboardEvent : public Event
	{
		int m_key;
		int m_action;

		KeyboardEvent()
		{
			m_type = EventType::Keyboard;
		}
		~KeyboardEvent() = default;
	};

	struct CloseEvent : public Event
	{
		CloseEvent()
		{
			m_type = EventType::Close;
		}
		~CloseEvent() = default;
	};
}
