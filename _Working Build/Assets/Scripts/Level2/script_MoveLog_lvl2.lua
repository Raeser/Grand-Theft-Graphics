local myDistanceX = 0.5
local myDistanceZ = 0.5
local offset1 = {-7.5, 4}
local offset2 = {-5, 4}
local offset3 = {-2.5, 4}
local offset4 = {0, 4}

local myHasCompletedObjective = false
local myOffsets = {offset1, offset2, offset3, offset4}
local myAmountOfTriggers = 1
local myTikiCounter = 0
local myTriggerConditionCounter = 4

function Init()
end

function OnEnter(id)

    if TikiIsInState(id, "Thrown") and myTikiCounter < myTriggerConditionCounter then
        myTikiCounter = myTikiCounter + 1
        KillTiki(id)
        
        local x, y, z = GetTriggerPosition()
        
        SpawnTikiAtPosition_Idle(x + (myOffsets[myTikiCounter][1] * myDistanceX), y, z - myDistanceZ + (myOffsets[myTikiCounter][2]* myDistanceX))
        
        if (myTikiCounter >= myTriggerConditionCounter) then
            IncrementObjectiveCounter()
            
            if GetObjectiveCounter() >= myAmountOfTriggers then
			    myHasCompletedObjective = true
				MoveLog()
            end
            
        end
            
    end
	
    if myHasCompletedObjective == true then
	KillStrandedTikis(id)
    end
end

function KillStrandedTikis(id)
    KillTiki(id)
end

function MoveLog(id)
   MoveObjectToPosition_Relative("MovableLog" ,0 ,0 ,-5 ,5 )
   RegisterCallback("MoveDone", "MovableLog", "func_1")
end

function func_1()
	Print("func_1")
	UnregisterCallback("MoveDone", "MovableLog")
	MoveObjectToPosition_Relative("MovableLog" ,0 ,-3.25 ,-4 ,5 )
	RegisterCallback("MoveDone", "MovableLog", "func_2")
end

function func_2()
	Print("func_2")
	UnregisterCallback("MoveDone", "MovableLog")
	MoveObjectToPosition_Relative("MovableLog" ,-10 ,0 ,3 ,5 )
	RotateObject("MovableLog", 0, 3.1415962/2.0, 0, 0.3)
	RegisterCallback("MoveDone", "MovableLog", "func_3")
end

function func_3()
	Print("func_3")
	UnregisterCallback("MoveDone", "MovableLog")
	--RotateObject("MovableLog", 0, 3.141/2.0, 0, 0.3)
	MoveObjectToPosition_Relative("MovableLog", -10 ,0 ,6 ,5)
	RegisterCallback("MoveDone", "MovableLog", "func_4")
end

function func_4()
	Print("func_4")
	UnregisterCallback("MoveDone", "MovableLog")
	--RotateObject("MovableLog", 0, 3.141/2.0, 0, 0.3)
	MoveObjectToPosition_Relative("MovableLog", -9 ,0 ,2.5 ,5)
	RegisterCallback("MoveDone", "MovableLog", "IMDONE")
end

function IMDONE()
		Print("WORK BITCH!")
		UnregisterCallback("MoveDone", "MovableLog")
		DisableColliderOnObject("LogNavBlocker")
		ResetObjectiveCounter()
		SetTikiLimit(4)
end


function OnExit(id)
end