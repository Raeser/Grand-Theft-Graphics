local myHasCompletedObjective = false

local myTikiLimit = 1
local myAmountOfTriggers = 3

function Init()
end

function OnEnter(id)	
	local count = GetTikiCount()
	
	if count >= myTikiLimit and myHasCompletedObjective == false then
		IncrementObjectiveCounter()
		myHasCompletedObjective = true
		
		if GetObjectiveCounter() >= myAmountOfTriggers then
			WinGame()
		end
	end
end

function OnExit(id)
	local count = GetTikiCount()
	
	if count < myTikiLimit and myHasCompletedObjective == true then
		DecrementObjectiveCounter()
		myHasCompletedObjective = false
	end
end