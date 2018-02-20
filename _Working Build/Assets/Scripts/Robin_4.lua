local myHasSpawned = false
local x, y, z 

function Init()

	SetTikiLimit(7)
end

function OnEnter(id)
	x, y, z = GetTriggerPosition()
	LoadSpriteAtPosition("../Assets/Sprites/tutorial/rightMouseTutorial.dds", x, y, z)
end

function OnExit(id)
end