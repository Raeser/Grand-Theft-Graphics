local myHasCompletedObjective = false

local myTikiLimit = 1
local myAmountOfTriggers = 2

function Init()
end

function OnEnter(id)	
	if IsTiki(id) and myHasCompletedObjective == false  then
		IncrementObjectiveCounter()
		KillTiki(id)
		x, y, z = GetTriggerPosition()
		SpawnTikiAtPosition_Idle(x, y - 0.5, z)
		myHasCompletedObjective = true
		local Count = GetObjectiveCounter()

		if GetObjectiveCounter() >= myAmountOfTriggers then
			Animate("gate2", "GateOpen")
			RegisterCallback("AnimationDone", "gate2", "IMDONE")
		end
		
	end
	
end

function IMDONE()
		UnregisterCallback("AnimationDone", "gate2")
		DisableColliderOnObject("LastGateCollider")
		ResetObjectiveCounter()
end

function OnExit(id)
end