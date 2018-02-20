#include "stdafx.h"
#include "CommandLineManager.h"


bool CommandLineManager::HasParameter(const char * aParameter)
{
	std::string s(aParameter);
	for (int i = 0; i < myParameters.Size(); ++i)
	{
		if (myParameters[i].name == s)
		{
			return true;
		}
	}
	return false;
}

bool CommandLineManager::HasArgument(const char * aParameter, const char * aArgument)
{

	std::string sp(aParameter);
	std::string sa(aArgument);
	for (int i = 0; i < myParameters.Size(); ++i)
	{
		if (myParameters[i].name == sp)
		{
			for (int j = 0; j < myParameters[i].arguments.Size(); ++j)
			{
				if (myParameters[i].arguments[j] == sa)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void CommandLineManager::Populate(char * argv)
{
	std::string s(argv);
	if (s.find("-") != -1)
	{
		Parameter p;
		p.name = s;
		p.arguments.Init(16);
		myParameters.Add(p);
	}
	else
	{
		if (myParameters.Size() > 0)
		{
			myParameters.GetLast().arguments.Init(16);
			myParameters.GetLast().arguments.Add(s);
		}
		else
		{
			//DL_WRITELOG() här ska vi skriva ett error;
		}
	}
}

CommandLineManager::CommandLineManager()
{
	myParameters.Init(64);
}


CommandLineManager::~CommandLineManager()
{
}
