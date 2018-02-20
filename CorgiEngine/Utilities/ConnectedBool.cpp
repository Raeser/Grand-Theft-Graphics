#include "stdafx.h"
#include "ConnectedBool.h"


ConnectedBool::ConnectedBool()
{
	myLinkedValue = nullptr;
}


ConnectedBool::~ConnectedBool()
{
}

void ConnectedBool::Link(ConnectedBool & a, ConnectedBool & b)
{
	a.myLinkedValue = &b.myValue;
	b.myLinkedValue = &a.myValue;
}


void ConnectedBool::operator=(bool aValue)
{
	myValue = aValue;
	*myLinkedValue = !myValue;
}

ConnectedBool::operator bool()
{
	return myValue;
}
