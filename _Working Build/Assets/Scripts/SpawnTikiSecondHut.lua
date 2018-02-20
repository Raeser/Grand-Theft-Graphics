local myHasSpawned = false
local x, y, z

function Init()
	x, y, z = GetTriggerPosition()
end

function OnEnter(id)	
        SetTikiLimit(2)	
		SpawnTikiAtPosition_Limited( x - 2, y, z)

end

function OnExit(id)
end
