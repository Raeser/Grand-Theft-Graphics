local x, y, z
local hasMoved = false

function Init()
end

function OnEnter(id)

    if hasMoved == false then
		MoveObjectToPosition_Relative("moveObject1" ,-20 ,0 ,0 ,5 )
		Print("Collision")
		hasMoved = true
		end
end

function OnExit(id)
end