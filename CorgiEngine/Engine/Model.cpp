#include "stdafx.h"
#include "Model.h"


CModel::CModel()
{
	myIsAnimated = false;
	myDefaultAnimationID = -1;
	myShouldCull = true;
	myModelData.myTextureData.myAlbedo = nullptr;
	myModelData.myTextureData.myNormalMap = nullptr;
	myModelData.myTextureData.myAmbientMap = nullptr;
	myModelData.myTextureData.myEmissiveMap = nullptr;
	myModelData.myTextureData.myMetalnessMap = nullptr;
	myModelData.myTextureData.myRoughnessMap = nullptr;
	myGeometryShaderID = 0;
}


CModel::~CModel()
{
}

bool CModel::Init(SModelData aModelData)
{
	myModelData = aModelData;
	if ((myBoundingBox.myMax - myBoundingBox.myMin).Length() > 50)
	{
		myShouldCull = false;
	}
	return true;

}

bool CModel::Release()
{

	
	//myModelData.myTextureData.myNormalMap->Release();
	//myModelData.myTextureData.myNormalMap = nullptr;
	//
	//myModelData.myTextureData.myAmbientMap->Release();
	//myModelData.myTextureData.myAmbientMap = nullptr;
	//
	//myModelData.myTextureData.myEmissiveMap->Release();
	//myModelData.myTextureData.myEmissiveMap = nullptr;
	//
	//myModelData.myTextureData.myMetalnessMap->Release();
	//myModelData.myTextureData.myMetalnessMap = nullptr;
	//
	//myModelData.myTextureData.myRoughnessMap->Release();
	//myModelData.myTextureData.myRoughnessMap = nullptr;


	//myModelData.myVertexData.myIndexBuffer->Release();
	//myModelData.myVertexData.myIndexBuffer = nullptr;
	//myModelData.myVertexData.myVertexBuffer->Release();
	//myModelData.myVertexData.myVertexBuffer = nullptr;
	return true;
}


void CModel::SetIsAnimated(bool aIsAnimated)
{
	myIsAnimated = aIsAnimated;
}

SVertexDataWrapper CModel::GetVertexData()
{
	return myModelData.myVertexData;
}

SShaderDataWrapper CModel::GetShaderData()
{
	return myModelData.myShaderData;
}

SLayoutDataWrapper CModel::GetLayoutData()
{
	return myModelData.myLayoutData;
}

STextureDataWrapper CModel::GetTextureData()
{
	return myModelData.myTextureData;
}

CModel::SBoundingBox & CModel::GetBoundingBox()
{
	return myBoundingBox;
}

void CModel::SetTextureEmissive(ID3D11ShaderResourceView * aViewToSet)
{
	myModelData.myTextureData.myEmissiveMap = aViewToSet;
}

bool CModel::IsAnimated()
{
	return myIsAnimated;
}

void CModel::SetShaders(long long aVertexShader, long long aPixelShader)
{
	myVertexShaderID = aVertexShader;
	myPixelShaderID = aPixelShader;
}

long long CModel::GetVertexShaderID()
{
	return myVertexShaderID;
}

long long CModel::GetPixelShaderID()
{
	return myPixelShaderID;
}

long long CModel::GetGeometryShaderID()
{
	return myGeometryShaderID;
}

void CModel::SetDefaultAnimationID(std::string aID)
{
	myDefaultAnimationID = aID;
}

const std::string & CModel::GetAnimationID() const
{
	return myDefaultAnimationID;
}
bool CModel::ShouldCull()
{
	return myShouldCull;
}
