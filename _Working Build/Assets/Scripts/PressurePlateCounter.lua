local myHasCompletedObjective = false

local myTikiLimit = 1
local myAmountOfTriggers = 3

function Init()
end

function OnEnter(id)
	
	local count = GetTikiCount()
	
	if count >= myTikiLimit and myHasCompletedObjective == false then
		IncrementObjectiveCounter()
		KillTiki(id)
		x, y, z = GetTriggerPosition()
		--SpawnTikiAtPosition(x, y, z)		--Will spawn a dead Tiki so we can control the amount of active Tikis the player got
		myHasCompletedObjective = true
		
		if GetObjectiveCounter() >= myAmountOfTriggers then
			Animate("gate1", "GateOpen")
			Print("Trying to open gate")
			RegisterCallback("AnimationDone", "gate1", "IMDONE", "YIPPIE!")
		end
	end
end

function IMDONE(str)
		Print(str)
		UnregisterCallback("AnimationDone", "gate1")
		ResetObjectiveCounter()
end

function OnExit(id)
	local count = GetTikiCount()
	
	if count < myTikiLimit and myHasCompletedObjective == true then
		DecrementObjectiveCounter()
		myHasCompletedObjective = false
	end
end