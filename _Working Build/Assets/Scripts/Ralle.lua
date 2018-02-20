x1 = 0.1
y1 = 0.6
z1 = 0.8
x2 = 0.1
y2 = 0.3
z2 = 0.2
prevx1 = 0.1
prevx2 = 0.1
shouldNotPrintTables = {10, 30, 40}
function Adam()
	return "Adam"
end
  
function Update_Ralle(one, two, three, four, five, six, seven)
	Print("I am gonna print something:", one, two, three, four, five, six, seven)
	PrintOnScreen("asdasdas", Adam(), one, four, seven, three, five, six)
	--DrawArrow(0,0,0,1,1,1)
	Print("all aboard :)")
end

function LongLoop()
	
	number = 0
	for i = 0, 500, 1
	do
		number = number + i
	end
	
	
	Print("I did a loop", number)
end

function pressFForLines()
	
	x1 = y1 + z1
	y1 = z1 + x1
	z1 = prevx1 + y1
	prevx1 = x1
	
	x2 = y2 + z2
	y2 = z2 + x2
	z2 = prevx2 + y2
	prevx2 = x2
	
	DrawArrow(x1,y1,z1,x2,y2,z2)
	DrawLine(2,2,1,3,3,5)
	Print("I did a arrow and löine")
end