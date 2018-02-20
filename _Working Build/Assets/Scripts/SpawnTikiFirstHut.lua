local myHasSpawned = false
local x, y, z

function Init()
	x, y, z = GetTriggerPosition()
end

function OnEnter(id)	
        SetTikiLimit(1)	
		SpawnTikiAtPosition_Limited( x + 3, y, z)
end

function OnExit(id)
end
