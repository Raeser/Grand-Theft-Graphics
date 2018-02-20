local myHasSpawned = false
local x, y, z

function Init()
		x, y, z = GetTriggerPosition()
end

function OnEnter(id)	
		SetTikiLimit(8)  -- Ta bort denna härifrån när vi har Triggers som styr TikiLimit
		SpawnTikiAtPosition_Limited(x, y, z)
		Print(id)

end

function OnExit(id)
end