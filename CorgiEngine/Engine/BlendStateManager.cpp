#include "stdafx.h"
#include "BlendStateManager.h"


CBlendStateManager::CBlendStateManager()
{
}


CBlendStateManager::~CBlendStateManager()
{
	for (BlendState& blendstate : myBlendStates)
	{
		blendstate.blendState->Release();
	}
}

bool CBlendStateManager::Init(CDXFramework* aFrameWork)
{
	myBlendStates.Init(EBlendState::eCount);
	for (int i = 0; i < EBlendState::eCount; i++)
	{
		myBlendStates.Add(BlendState());
	}
	myFrameWork = aFrameWork;
	if (!CreateBlendStates())
	{
		return false;
	}
	SetBlendState(EBlendState::EBlendState_Disabled);
	return true;
}

bool CBlendStateManager::SetBlendState(EBlendState aState)
{
	myFrameWork->GetContext()->OMSetBlendState(myBlendStates[aState].blendState, myBlendStates[aState].blendFactor, 0xffffffff);
	return true;
}

bool CBlendStateManager::SetDefaultState()
{
	myFrameWork->GetContext()->OMSetBlendState(myBlendStates[EBlendState::EBlendState_Disabled].blendState, myBlendStates[EBlendState::EBlendState_Disabled].blendFactor, 0xffffffff);
	return true;
}


bool CBlendStateManager::CreateBlendStates()
{

	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	HRESULT result = S_OK;
	D3D11_BLEND_DESC blendStateDescription;


	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	result = myFrameWork->GetDevice()->CreateBlendState(&blendStateDescription, &myBlendStates[EBlendState::EBlendState_Disabled].blendState);
	if (FAILED(result))
	{
		return false;
	}
	myBlendStates[EBlendState::EBlendState_Disabled].blendFactor[0] = blendFactor[0];
	myBlendStates[EBlendState::EBlendState_Disabled].blendFactor[1] = blendFactor[1];
	myBlendStates[EBlendState::EBlendState_Disabled].blendFactor[2] = blendFactor[2];
	myBlendStates[EBlendState::EBlendState_Disabled].blendFactor[3] = blendFactor[3];


	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	result = myFrameWork->GetDevice()->CreateBlendState(&blendStateDescription, &myBlendStates[EBlendState::EBlendState_Alphablend].blendState);



	if (FAILED(result))
	{
		return false;
	}
	myBlendStates[EBlendState::EBlendState_Alphablend].blendFactor[0] = blendFactor[0];
	myBlendStates[EBlendState::EBlendState_Alphablend].blendFactor[1] = blendFactor[1];
	myBlendStates[EBlendState::EBlendState_Alphablend].blendFactor[2] = blendFactor[2];
	myBlendStates[EBlendState::EBlendState_Alphablend].blendFactor[3] = blendFactor[3];

	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	result = myFrameWork->GetDevice()->CreateBlendState(&blendStateDescription, &myBlendStates[EBlendState::EBlendState_Additativeblend].blendState);
	if (FAILED(result))
	{
		return false;
	}
	myBlendStates[EBlendState::EBlendState_Additativeblend].blendFactor[0] = blendFactor[0];
	myBlendStates[EBlendState::EBlendState_Additativeblend].blendFactor[1] = blendFactor[1];
	myBlendStates[EBlendState::EBlendState_Additativeblend].blendFactor[2] = blendFactor[2];
	myBlendStates[EBlendState::EBlendState_Additativeblend].blendFactor[3] = blendFactor[3];


	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	blendStateDescription.AlphaToCoverageEnable = FALSE;
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	result = myFrameWork->GetDevice()->CreateBlendState(&blendStateDescription, &myBlendStates[EBlendState::EBlendState_SpriteBlend].blendState);
	if (FAILED(result))
	{
		return false;
	}
	myBlendStates[EBlendState::EBlendState_SpriteBlend].blendFactor[0] = blendFactor[0];
	myBlendStates[EBlendState::EBlendState_SpriteBlend].blendFactor[1] = blendFactor[1];
	myBlendStates[EBlendState::EBlendState_SpriteBlend].blendFactor[2] = blendFactor[2];
	myBlendStates[EBlendState::EBlendState_SpriteBlend].blendFactor[3] = blendFactor[3];

	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	blendStateDescription.AlphaToCoverageEnable = TRUE;
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	result = myFrameWork->GetDevice()->CreateBlendState(&blendStateDescription, &myBlendStates[EBlendState::EBlendState_AddAlphaSpriteBlend].blendState);
	if (FAILED(result))
	{
		return false;
	}
	myBlendStates[EBlendState::EBlendState_AddAlphaSpriteBlend].blendFactor[0] = blendFactor[0];
	myBlendStates[EBlendState::EBlendState_AddAlphaSpriteBlend].blendFactor[1] = blendFactor[1];
	myBlendStates[EBlendState::EBlendState_AddAlphaSpriteBlend].blendFactor[2] = blendFactor[2];
	myBlendStates[EBlendState::EBlendState_AddAlphaSpriteBlend].blendFactor[3] = blendFactor[3];


	return true; 
	
}