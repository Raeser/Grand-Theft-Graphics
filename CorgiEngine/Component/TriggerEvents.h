#pragma once
namespace TriggerEvent
{
	enum class EEventType
	{
		Unknown,
		PlayerWin,
		PlayerDie,
		count
	};

	static const char* EventStrings[] = 
	{
		"Unknown",
		"PlayerWin",
		"PlayerDie",
	};

	enum class EColliderTypeTrigger
	{
		OnEnter,
		OnCollision,
		OnExit,
		count
	};
}



