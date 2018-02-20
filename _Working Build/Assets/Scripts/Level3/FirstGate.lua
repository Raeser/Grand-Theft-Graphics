local myHasCompletedObjective = false

local myTriggerConditionCounter = 3

function Init()
end

function OnEnter(id)	

	if TikiIsInState(id, "Thrown") and myHasCompletedObjective == false then
        KillTiki(id)
		x, y, z = GetTriggerPosition()
		SpawnTikiAtPosition_Idle(x, y - 0.5, z)
		myHasCompletedObjective = true
		
		IncrementObjectiveCounter()
		if GetObjectiveCounter() >= myTriggerConditionCounter then
			Animate("gate1", "GateOpen")
			RegisterCallback("AnimationDone", "gate1", "IMDONE", "YIPPIE!")
		end
	end
end

function IMDONE(str)
		UnregisterCallback("AnimationDone", "gate1")
		DisableColliderOnObject("FirstGateCollider")
		ResetObjectiveCounter()
end

function OnExit(id)
end