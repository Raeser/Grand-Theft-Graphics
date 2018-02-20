
local x, y, z
local hasMoved = false
local myTikiLimit = 2
local throwCounter = 0

function Init()
end

function OnEnter(id)
	if  hasMoved == false then
		local count = GetTikiCount()
		
		if TikiIsInState(id, "Thrown") then
			throwCounter = throwCounter +1
		end

		if throwCounter >= myTikiLimit and count >= myTikiLimit then
			MoveObjectToPosition_Relative("moveObject2" ,-25 ,0 ,0 ,5 )
			DisableColliderOnObject("FirstMoveableBlockCollider")
			hasMoved = true
			end
	end		
end

function OnExit(id)
end