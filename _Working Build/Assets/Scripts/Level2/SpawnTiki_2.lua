
local x, y, z

function Init()
		x, y, z = GetTriggerPosition()
end

function OnEnter(id)
        SetTikiLimit(4)	
		SpawnTikiAtPosition_Limited( x, y, z)
end

function OnExit(id)
end