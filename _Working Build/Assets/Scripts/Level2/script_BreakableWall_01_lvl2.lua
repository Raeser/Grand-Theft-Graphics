local myHasCompletedObjective = false

local myTikiLimit = 4
local myAmountOfTriggers = 1
local throwCounter = 0

function Init()
end

function OnEnter(id)
	
	local count = GetTikiCount()
	
	if TikiIsInState(id, "Thrown") then
		throwCounter = throwCounter +1
	end
	
	if throwCounter >= myTikiLimit and myHasCompletedObjective == false then
		IncrementObjectiveCounter()
		x, y, z = GetTriggerPosition()
		myHasCompletedObjective = true
		
		local obj = GetObjectiveCounter()

		if GetObjectiveCounter() >= myAmountOfTriggers then
			Animate("breakableWall01", "BreakableWallBreak")		
			RegisterCallback("AnimationDone", "breakableWall01", "IMDONE", "YIPPIE!")
		end
	end
end

function IMDONE(str)
		Print(str)	
		UnregisterCallback("AnimationDone", "breakableWall01")
		DisableColliderOnObject("BreakableWall01_lvl2")
		ResetObjectiveCounter()
end

function OnExit(id)

end