local myHasCompletedObjective = false

local myTikiLimit = 1
local myAmountOfTriggers = 1

local throwCounter = 0
local objectiveDone = false

function Init()
end

function OnEnter(id)
	
	local count = GetTikiCount()
	
	if TikiIsInState(id, "Thrown") then
	
	throwCounter = throwCounter +1
	
	end
	
	if throwCounter >= myTikiLimit and count >= myTikiLimit and myHasCompletedObjective == false then
		IncrementObjectiveCounter()
		x, y, z = GetTriggerPosition()
		myHasCompletedObjective = true
		
		local obj = GetObjectiveCounter()
		Print(obj)			
		if GetObjectiveCounter() >= myAmountOfTriggers then
			Animate("breakwall1_tut", "BreakableWallBreak")
			Print("Trying to break wall")
			RegisterCallback("AnimationDone", "breakwall1_tut", "IMDONE", "YIPPIE!")
		end
	end
end

function IMDONE(str)
	
		Print(str)
		DisableColliderOnObject("tutBreakWall1NavBlocker")
		UnregisterCallback("AnimationDone", "breakwall1_tut")
		ResetObjectiveCounter()
end

function OnExit(id)

	if TikiIsInState(id, "Thrown") then
	
		throwCounter = throwCounter - 1
	
	end	
end