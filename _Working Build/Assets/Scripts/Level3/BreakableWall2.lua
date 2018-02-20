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
	
	if throwCounter >= myTikiLimit and count >= myTikiLimit and myHasCompletedObjective == false then
		x, y, z = GetTriggerPosition()
		myHasCompletedObjective = true
		

			Animate("breakableWall_2", "BreakableWallBreak")
			DisableColliderOnObject("TESTcollider")
			RegisterCallback("AnimationDone", "breakableWall_2", "IMDONE", "YIPPIE!")

	end
end

function IMDONE(str)
		Print(str)
		UnregisterCallback("AnimationDone", "breakableWall_2")
end

function OnExit(id)
	if TikiIsInState(id, "Thrown") then
		throwCounter = throwCounter - 1
	end
end