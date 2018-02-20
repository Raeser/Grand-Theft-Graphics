
local x, y, z

function Init()
		x, y, z = GetTriggerPosition()
end

function OnEnter(id)	
		SpawnTikiAtPosition_Limited(x + 2, y, z -2)
end

function OnExit(id)
end