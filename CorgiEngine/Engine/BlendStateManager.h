#pragma once
#include "..\Utilities\GrowingArray.h"

class CDXFramework;
struct  D3D11_BLEND_DESC;
struct ID3D11BlendState;

enum EBlendState
{
	EBlendState_Disabled,
	EBlendState_Alphablend,
	EBlendState_Additativeblend,
	EBlendState_SpriteBlend,
	EBlendState_AddAlphaSpriteBlend,
	eCount
};

class CBlendStateManager
{
public:
	CBlendStateManager();
	~CBlendStateManager();

	bool Init(CDXFramework* aFrameWork);
	bool SetBlendState(EBlendState aState);
	bool SetDefaultState();
private:
	bool CreateBlendStates();
	struct BlendState
	{
		ID3D11BlendState* blendState;
		float blendFactor[4];
	};
	CDXFramework* myFrameWork;
	Corgi::GrowingArray<BlendState, int> myBlendStates;
};

