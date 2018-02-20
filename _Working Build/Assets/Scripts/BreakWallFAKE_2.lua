local x, y, z
local hasMoved = false

function Init()
end

function OnEnter(id)

    if hasMoved == false then
		MoveObjectToPosition_Relative("breakWall5" ,0 ,-15 ,0 ,5 )
		MoveObjectToPosition_Relative("breakWall6" ,0 ,-15 ,0 ,5 )
		MoveObjectToPosition_Relative("breakWall7" ,0 ,-15 ,0 ,5 )
		MoveObjectToPosition_Relative("breakWall8" ,0 ,-15 ,0 ,5 )
		Print("Collision")
		hasMoved = true
		end
end

function OnExit(id)
end