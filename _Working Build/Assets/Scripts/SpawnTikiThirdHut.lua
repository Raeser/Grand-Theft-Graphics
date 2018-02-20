local myHasSpawned = false
local x, y, z

function Init()
end

function OnEnter(id)	
        SetTikiLimit(3)	
		x, y, z = GetPlayerPosition()
		SpawnTikiAtPosition_Limited(x, 0, z)

end

function OnExit(id)
end
