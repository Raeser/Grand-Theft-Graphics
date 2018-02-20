#pragma once
class ConnectedBool
{
public:
	ConnectedBool();
	~ConnectedBool();

	static void Link(ConnectedBool& a, ConnectedBool& b);

	void operator=(bool aValue);
	operator bool();

private:
	bool myValue;
	bool* myLinkedValue;
};

