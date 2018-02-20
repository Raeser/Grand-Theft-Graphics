local myHasSpawned = false
local x, y, z 

function Init()
end

function OnEnter(id)
	if myHasSpawned == false then
		myHasSpawned = true
		x, y, z = GetTriggerPosition()
		LoadSpriteAtPosition("../Assets/Sprites/tutorial/followTutorial.dds",x - 10, y, z)

	end
end
function OnExit(id)
end