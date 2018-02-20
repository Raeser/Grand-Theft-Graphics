local x, y, z
local hasMoved = false

function Init()
end

function OnEnter(id)

    if hasMoved == false then
		MoveLog()
		Print("Collision")
		hasMoved = true
    end
end

function MoveLog(id)
   MoveObjectToPosition_Relative("MovableLog" ,0 ,0 ,-5 ,5 )
   Sleep(1000, "FallDown", x, y, z) 
end

function FallDown(a, b, c)
   MoveObjectToPosition_Relative("MovableLog" ,0 ,-5 ,0 ,5 )
   Sleep(1000, "FirstMove", x, y, z) 
end

function FirstMove(a, b, c)
   MoveObjectToPosition_Relative("MovableLog" ,-15 ,0 ,0 ,5 )
   Sleep(3000, "SecondMove", x, y, z) 
end

function SecondMove(a, b, c)
   MoveObjectToPosition_Relative("MovableLog" ,0 ,0 ,7 ,5 )

end


function OnExit(id)
end