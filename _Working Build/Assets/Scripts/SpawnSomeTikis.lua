local myHasSpawned = false
local x, y, z 

function Init()
	x, y, z = GetTriggerPosition()
	LoadSpriteAtPosition("../Assets/Sprites/borb.dds", x, y, z)
	SetTikiLimit(7)
end

function OnEnter(id)
	SpawnTikiAtPosition_Limited(x + 10, y, z)
end

function OnExit(id)
end