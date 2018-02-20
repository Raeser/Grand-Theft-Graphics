local x, y, z
local hasMoved = false

function Init()
end

function OnEnter(id)

    if hasMoved == false then
		MoveObjectToPosition_Relative("movWall01" ,0 ,-15 ,0 ,5 )
		MoveObjectToPosition_Relative("movWall02" ,0 ,-15 ,0 ,5 )
		Print("Collision")
		hasMoved = true
		end
end

function OnExit(id)
end