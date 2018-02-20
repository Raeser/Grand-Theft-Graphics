local x, y, z
local hasMoved = false

function Init()
end

function OnEnter(id)

    if hasMoved == false then
		MoveObjectToPosition_Relative("breakWall1" ,0 ,-15 ,0 ,5 )
		MoveObjectToPosition_Relative("breakWall2" ,0 ,-15 ,0 ,5 )
		MoveObjectToPosition_Relative("breakWall3" ,0 ,-15 ,0 ,5 )
		MoveObjectToPosition_Relative("breakWall4" ,0 ,-15 ,0 ,5 )
		Print("Collision")
		hasMoved = true
		end
end

function OnExit(id)
end