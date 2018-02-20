#pragma once
#include "Component.h"


class CCollisionComponent :
	public CComponent
{
public:
	CCollisionComponent();
	~CCollisionComponent();

	void Init();
	void Update() override;
	void FetchData() override;
	void SendData() override;
	void Release() override;

private:
};


