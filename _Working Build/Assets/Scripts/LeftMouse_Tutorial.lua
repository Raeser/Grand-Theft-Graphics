local myHasSpawned = false
local x, y, z 

function Init()
end

function OnEnter(id)
	if myHasSpawned == false then
		myHasSpawned = true
		x, y, z = GetTriggerPosition()
		Sleep(3500, "Draw", x, y, z)
	end
end

function OnExit(id)
end

function Draw(a, b, c)
		LoadSpriteAtPosition("../Assets/Sprites/tutorial/leftMouseTutorial.dds", a, b, c)
end