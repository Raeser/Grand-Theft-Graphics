#include "stdafx.h"

void SetConsoleColor(eConsoleColor aColor)
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!hConsole)
	{
		return;
	}
	SetConsoleTextAttribute(hConsole, static_cast<WORD>(aColor));
}