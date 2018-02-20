#pragma once

enum class CollisionLayer
{
	Player = 1,
	Minion = 2,
	Trigger = 4,
	Enemy = 8
};

enum class EntityType
{
	Player,
	Minion,
	Trigger,
	Enemy

};

enum class ColliderType
{
	Hit,
	Trigger
};