
local x, y, z
local hasMoved = false
local myTikiLimit = 3
local throwCounter = 0

function Init()
end

function OnEnter(id)
	if  hasMoved == false then	
		local count = GetTikiCount()
		
		if TikiIsInState(id, "Thrown") then
			throwCounter = throwCounter +1
			Print("In")
		end
		
		if throwCounter >= myTikiLimit and count >= myTikiLimit then
			MoveObjectToPosition_Relative("moveObject3", -22, 0, 0, 5)
			DisableColliderOnObject("LeftMoveableBlockCollider")
			hasMoved = true
		end
	end
end

function OnExit(id)
end


