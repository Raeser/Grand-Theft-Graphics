local myHasCompletedObjective = false

local myTikiLimit = 3
local myAmountOfTriggers = 1

local throwCounter = 0

function Init()
end

function OnEnter(id)
	
	local count = GetTikiCount()
	
	if TikiIsInState(id, "Thrown") then
	
	throwCounter = throwCounter +1
	
	end
	
	if throwCounter >= myTikiLimit and count >= myTikiLimit and myHasCompletedObjective == false then
		x, y, z = GetTriggerPosition()
		myHasCompletedObjective = true				
		Animate("breakwall2_tut", "BreakableWallBreak")		
		Print("Trying to break wall")
		RegisterCallback("AnimationDone", "breakwall2_tut", "IMDONE", "YIPPIE!")
	end
end

function IMDONE(str)
		Print(str)
		DisableColliderOnObject("tutBreakWall2NavBlocker")
		UnregisterCallback("AnimationDone", "breakwall2_tut")
		ResetObjectiveCounter()
end

function OnExit(id)

end