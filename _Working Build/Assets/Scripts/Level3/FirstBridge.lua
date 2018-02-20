local myDistanceX = 0.5
local myDistanceZ = 0.5
local offset1 = {1, 0}
local offset2 = {-0.49999846794, 0.86602628831}
local offset3 = {-0.50000306409, -0.86602363471}

local myOffsets = {offset1, offset2, offset3}
local myAmountOfTriggers = 1
local myTikiCounter = 0
local myTriggerConditionCounter = 1

function Init()
end

function OnEnter(id)
    
    
    if TikiIsInState(id, "Thrown") and myTikiCounter < myTriggerConditionCounter then
        myTikiCounter = myTikiCounter + 1
        KillTiki(id)
        
        local x, y, z = GetTriggerPosition()
        SpawnTikiAtPosition_Idle(x + (myOffsets[myTikiCounter][1] * myDistanceX), y, z - myDistanceZ + (myOffsets[myTikiCounter][2]* myDistanceX))

         Animate("bridge1", "BridgeOpen")
         RegisterCallback("AnimationDone", "bridge1", "IMDONE", "YIPPIE!") 
    end
end

function IMDONE(str)
		DisableColliderOnObject("FirstBridgeCollider")
        UnregisterCallback("AnimationDone", "bridge1")
end

function OnExit(id)

end