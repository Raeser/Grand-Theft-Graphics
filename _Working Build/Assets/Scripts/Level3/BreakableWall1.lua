local myHasCompletedObjective = false

local myTikiLimit = 6
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
		IncrementObjectiveCounter()
		x, y, z = GetTriggerPosition()
		
		local obj = GetObjectiveCounter()

			Animate("breakableWall_1", "BreakableWallBreak")
			DisableColliderOnObject("BreakableWallCollider")
			DisableColliderOnObject("BreakableWallCollider2")
			DisableColliderOnObject("BreakableWallCollider3")
			DisableColliderOnObject("BreakableWallCollider4")
			DisableColliderOnObject("BreakableWallCollider5")
			myHasCompletedObjective = true
			RegisterCallback("AnimationDone", "breakableWall_1", "IMDONE", "YIPPIE!")

	end
end

function IMDONE(str)
		Print(str)
		UnregisterCallback("AnimationDone", "breakableWall_1")

end

function OnExit(id)
	if TikiIsInState(id, "Thrown") then
		throwCounter = throwCounter - 1
	end
end