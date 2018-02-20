local myStartedAnim = false
function Init()
end

function OnEnter(id)	
	if myStartedAnim == false then
		myStartedAnim = true 
		Animate("StureGate","GateOpen")
		Print("askjfbasfsaspaofj")
		RegisterCallback("AnimationDone", "StureGate", "IMDONE", "YIPPIE!")
	end
end

function OnExit(id)
end

function IMDONE(str)
		Print(str)
		UnregisterCallback("AnimationDone", "StureGate")
end
