#pragma once
#include "GrowingArray.h"

class CommandLineManager
{
public:
	~CommandLineManager();

	static CommandLineManager& GetInstance()
	{
		static CommandLineManager me;
		return me;
	}

	bool HasParameter(const char* aParameter);
	bool HasArgument(const char* aParameter, const char* aArgument);

	void Populate(char* argv);

private:
	CommandLineManager();

	struct Parameter
	{
		std::string name;
		Corgi::GrowingArray<std::string> arguments;
	};

	Corgi::GrowingArray<Parameter> myParameters;




};

