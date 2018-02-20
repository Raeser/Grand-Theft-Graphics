local myHasSpawned = false
local x, y, z

function Init()
	x, y, z = GetTriggerPosition()
	LoadSpriteAtPosition("../Assets/Sprites/borb.dds", x, y, z)
end

function OnEnter(id)	

	if(IsTiki(id)) then
		
		x, y, z = GetTriggerPosition()
		--SpawnTikiAtPosition_Idle(x, y, z)
		if TikiIsInState(id, "Thrown") then
			KillTiki(id)
			SpawnTikiAtPosition_Idle(x, y, z)
			Print("BLOODY MURDER");
			PlaySoundAtPosition("Play_Stone_Grind", x, y, z)
		end
	end

	
end

function OnExit(id)
end