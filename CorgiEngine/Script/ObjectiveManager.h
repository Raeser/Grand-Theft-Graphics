#pragma once
class CObjectiveManager
{
public:
	~CObjectiveManager();

	static CObjectiveManager& GetInstance()
	{
		static CObjectiveManager me;
		return me;
	}

	void IncrementObjectiveCounter();
	void DecrementObjectiveCounter();
	int GetObjectiveCounter() const;
	int Reset();
private:
	CObjectiveManager();
	int myObjectiveCounter;
};

