local myDistanceX = 0.5
local myDistanceZ = 0.5
local offset1 = {1, 0}
local offset2 = {-0.49999846794, 0.86602628831}
local offset3 = {-0.50000306409, -0.86602363471}

local myOffsets = {offset1, offset2, offset3}
local myAmountOfTriggers = 1
local myTikiCounter = 0
local myTriggerConditionCounter = 2
local myTriggerStarted = false

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
            
            if GetObjectiveCounter() >= myAmountOfTriggers and myTriggerStarted == false then
			myTriggerStarted = true
			Print("MovingPillars")
			MovePillar01()
            end
            
        end
            
    end
end


function MovePillar01(id)
         Print("moving1")
		MoveObjectToPosition_Relative("PillarCluster01" ,0 ,-4 ,0 ,2 )
		Sleep(1000, "MovePillar02", x, y, z)
		--RegisterCallback("MoveDone", "PillarCluster02", "MovePillar02")
	
end

function MovePillar02(a, b, c)
         Print("moving2")
        --UnregisterCallback("MoveDone", "MovePillar01") 
		MoveObjectToPosition_Relative("PillarCluster02" ,0 ,-4 ,0 ,2 )
		Sleep(1000, "MovePillar03", x, y, z)
		--RegisterCallback("MoveDone", "PillarCluster03", "MovePillar03")
	
end

function MovePillar03(a, b, c)
         Print("moving3")
        --UnregisterCallback("MoveDone", "MovePillar02")
		MoveObjectToPosition_Relative("PillarCluster03" ,0 ,-4 ,0 ,2 )
		Sleep(1000, "MovePillar07", x, y, z)
		--RegisterCallback("MoveDone", "PillarCluster07", "MovePillar07")
	
end

function MovePillar07(a, b, c)
         Print("moving7")
        --UnregisterCallback("MoveDone", "MovePillar03")         
		MoveObjectToPosition_Relative("PillarCluster07" ,0 ,-4 ,0 ,2 )
		Sleep(1000, "MovePillar08", x, y, z)
		--RegisterCallback("MoveDone", "PillarCluster08", "MovePillar08")	
end

function MovePillar08(a, b, c)
         Print("moving8")
        --UnregisterCallback("MoveDone", "MovePillar07")         
		MoveObjectToPosition_Relative("PillarCluster08" ,0 ,-4 ,0 ,2 )
		Sleep(1000, "MovePillar09", x, y, z)
		--RegisterCallback("MoveDone", "PillarCluster09", "MovePillar09")	
end

function MovePillar09(a, b, c)
         Print("moving9")
        --UnregisterCallback("MoveDone", "MovePillar08")         
		MoveObjectToPosition_Relative("PillarCluster09" ,0 ,-4 ,0 ,2 )
		Sleep(1000, "MovePillar04", x, y, z)
		--RegisterCallback("MoveDone", "PillarCluster04", "MovePillar04")	
end

function MovePillar04(a, b, c)
         Print("moving4")
        --UnregisterCallback("MoveDone", "MovePillar09")         
		MoveObjectToPosition_Relative("PillarCluster04" ,0 ,-4 ,0 ,2 )
		Sleep(1000, "MovePillar05", x, y, z)
		--RegisterCallback("MoveDone", "PillarCluster05", "MovePillar05")	
end

function MovePillar05(a, b, c)
         Print("moving5")
        --UnregisterCallback("MoveDone", "MovePillar04")         
		MoveObjectToPosition_Relative("PillarCluster05" ,0 ,-4 ,0 ,2 )
	    Sleep(1000, "MovePillar06", x, y, z)
		--RegisterCallback("MoveDone", "PillarCluster06", "MovePillar06")		
end

function MovePillar06(a, b, c)
         Print("moving6")
        --UnregisterCallback("MoveDone", "MovePillar05")         
		MoveObjectToPosition_Relative("PillarCluster06" ,0 ,-4 ,0 ,2 )
		--RegisterCallback("MoveDone","PillarCluster06","IMDONE")
		IMDONE()

end

function IMDONE()
        Print("ImDone")
        UnregisterCallback("MoveDone","MovePillar06")
		DisableColliderOnObject("Pillar01_Lvl2Blocker02")
	    DisableColliderOnObject("Pillar01_Lvl2Blocker01")
        ResetObjectiveCounter()
end

function OnExit(id)
end