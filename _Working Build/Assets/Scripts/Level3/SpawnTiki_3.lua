
local x, y, z

function Init()
		x, y, z = GetTriggerPosition()
end

function OnEnter(id)	
		SpawnTikiAtPosition_Limited(x, y, z + 3)
end

function OnExit(id)
end