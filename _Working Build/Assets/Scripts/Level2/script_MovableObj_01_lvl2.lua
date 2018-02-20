local myHasCompletedObjective = false
local x, y, z
local hasMoved = false
local myTikiLimit = 2
local throwCounter = 0

function Init()
end

function OnEnter(id)

if hasMoved == false then 

	local count = GetTikiCount()
	
	if TikiIsInState(id, "Thrown") then
		throwCounter = throwCounter +1
	end

	if throwCounter >= myTikiLimit and count >= myTikiLimit and myHasCompletedObjective == false then
		MoveObjectToPosition_Relative("MovableObj_01_lvl2" ,0 ,0 ,14 ,3 )
		DisableColliderOnObject("MovObj1Coll")
		hasMoved = true
		end
		end
end

function OnExit(id)
	local count = GetTikiCount()
	
	if count < myTikiLimit and myHasCompletedObjective == true then
		DecrementObjectiveCounter()
		myHasCompletedObjective = false
	end
end

