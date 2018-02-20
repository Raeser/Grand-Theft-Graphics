local myHasCompletedObjective = false
local x, y, z
local hasMoved = false
local myTikiLimit = 2
local throwCounter = 0

function Init()
end

function OnEnter(id)
	local count = GetTikiCount()
	
	if TikiIsInState(id, "Thrown") then
		throwCounter = throwCounter +1
	end

	if throwCounter >= myTikiLimit and count >= myTikiLimit and myHasCompletedObjective == false then
		MoveObjectToPosition_Relative("MovableObj_02_lvl2" ,0 ,0 ,-12 ,3 )
		DisableColliderOnObject("MovObj2Coll")
		--hasMoved = true
		end
end

function OnExit(id)
	local count = GetTikiCount()
	
	if count < myTikiLimit and myHasCompletedObjective == true then
		DecrementObjectiveCounter()
		myHasCompletedObjective = false
	end
end

