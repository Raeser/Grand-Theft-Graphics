local myDistanceX = 0.5
local myDistanceZ = 0.5
local offset1 = {1, 0}
local offset2 = {-0.49999846794, 0.86602628831}
local offset3 = {-0.50000306409, -0.86602363471}

local myOffsets = {offset1, offset2, offset3}
local myAmountOfTriggers = 1
local myTikiCounter = 0
local myTriggerConditionCounter = 3

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
			Animate("gate01", "GateOpen")
			Print("Trying to open gate")
			RegisterCallback("AnimationDone", "gate01", "IMDONE", "YIPPIE!")
            end
            
        end
            
    end
end

function IMDONE()
        Print(str)
        UnregisterCallback("AnimationDone", "gate01")
		DisableColliderOnObject("Gate01_lvl2Blocker1")
		DisableColliderOnObject("Gate01_lvl2Blocker2")
		DisableColliderOnObject("Gate01_lvl2Blocker3")
		DisableColliderOnObject("Gate01_lvl2Blocker4")
		DisableColliderOnObject("Gate01_lvl2Blocker5")
		DisableColliderOnObject("Gate01_lvl2Blocker6")
        ResetObjectiveCounter()
		SetTikiLimit(6)
end

function OnExit(id)

end