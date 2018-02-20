
local x, y, z

function Init()
		x, y, z = GetTriggerPosition()
end

function OnEnter(id)	
		SpawnTikiAtPosition_Limited( x - 4, y, z)
end

function OnExit(id)
end