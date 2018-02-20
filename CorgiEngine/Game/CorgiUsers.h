#pragma once
//#include <iostream>
#include <vector>

enum class eUser
{
	SPMagnus,
	Robin,
	Sebastian,
	Axel,
	Lukas,
	Annette,
	Jonas,
	Martin,
	TAMagnus,
	Hampus,
	Anton,
	Hini,
	Tomas,
	defualt,
};

const struct SUserString
{
	std::vector<char*> users =
	{
		"SPMagnus",
		"Robin",
		"Sebastian",
		"Axel",
		"Lukas",
		"Annette",
		"Jonas",
		"Martin",
		"TAMagnus",
		"Hampus",
		"Anton",
		"Hini",
		"Tomas",
		"Default",
	};
};
