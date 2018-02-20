local myHasCompletedObjective = false

local myTikiLimit = 1
local myAmountOfTriggers = 1

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
		Print("REREEEEEEEEE")
		Print(Count)
		if GetObjectiveCounter() >= myAmountOfTriggers then
			Animate("gate3", "GateOpen")
			RegisterCallback("AnimationDone", "gate3", "IMDONE")
		end
	end
end

function IMDONE()
		Print("hej")
		UnregisterCallback("AnimationDone", "gate3")
		DisableColliderOnObject("LastGateCollider")
		ResetObjectiveCounter()
end

function OnExit(id)
end