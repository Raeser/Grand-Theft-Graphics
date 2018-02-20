#include "stdafx.h"
#include "Sprite.h"


CSprite::CSprite()
{
}


CSprite::~CSprite()
{
	//mySpriteData.myLayoutData.myInputLayout->Release();
	//mySpriteData.myLayoutData.myInputLayout = nullptr;
	//
	//mySpriteData.myShaderData.myPixelShader->Release();
	//mySpriteData.myShaderData.myPixelShader = nullptr;
	//
	//
	//mySpriteData.myShaderData.myVertexShader->Release();
	//mySpriteData.myShaderData.myVertexShader = nullptr;
	//
	//mySpriteData.myTextureData.myAlbedo->Release();
	//mySpriteData.myTextureData.myAlbedo = nullptr;
	//
	//mySpriteData.myVertexData.myIndexBuffer->Release();
	//mySpriteData.myVertexData.myIndexBuffer = nullptr;
	//
	//mySpriteData.myVertexData.myVertexBuffer->Release();
	//mySpriteData.myVertexData.myVertexBuffer = nullptr;
}

bool CSprite::Init(SSpriteData aModelData)
{
	mySpriteData = aModelData;
	return true;
}

bool CSprite::SetSpriteData(SSpriteData aModelData)
{
	mySpriteData = aModelData;
	return true;
}

const Vector2f & CSprite::GetOriginalSize()
{
	return myOriginalSize;
}

void CSprite::SetOriginalSize(Vector2f aSize)
{
	myOriginalSize = aSize;
}
