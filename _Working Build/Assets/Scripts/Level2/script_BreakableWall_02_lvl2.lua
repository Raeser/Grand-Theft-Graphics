local myHasCompletedObjective = false

local myTikiLimit = 2
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
			Animate("breakableWall02", "BreakableWallBreak")
			DisableColliderOnObject("BreakableWall02_lvl2")

			RegisterCallback("AnimationDone", "breakableWall02", "IMDONE", "YIPPIE!")
		end
	end
end

function IMDONE(str)
		Print(str)
		UnregisterCallback("AnimationDone", "breakableWall02")
		ResetObjectiveCounter()
		SetTikiLimit(4)
end

function OnExit(id)

end