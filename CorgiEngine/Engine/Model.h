#pragma once
#include "D3D11DataWrappers.h"
#include "cAnimationController.h"
#include "Skeleton.h"

struct aiScene;

class CModel
{
public:
	friend CModelLoader;
	CModel();
	~CModel();

	struct SBoundingBox
	{
		Corgi::Vector3<float> myMax;
		Corgi::Vector3<float> myMin;
	};
	struct SModelData
	{
		SVertexDataWrapper myVertexData;
		SShaderDataWrapper myShaderData;
		SLayoutDataWrapper myLayoutData;
		STextureDataWrapper myTextureData;
	};

	bool Init(SModelData aModelData);
	bool Release();
	void SetIsAnimated(bool aIsAnimated);
	SVertexDataWrapper GetVertexData();
	SShaderDataWrapper GetShaderData();
	SLayoutDataWrapper GetLayoutData();
	STextureDataWrapper GetTextureData();
	SBoundingBox& GetBoundingBox();

	void SetTextureEmissive(ID3D11ShaderResourceView* aViewToSet);
	bool IsAnimated();
	void SetShaders(long long aVertexShader, long long aPixelShader);
	long long GetVertexShaderID();
	long long GetPixelShaderID();
	long long GetGeometryShaderID();
	void SetDefaultAnimationID(std::string aID);
	const std::string& GetAnimationID() const;
	bool ShouldCull();


private:
	SModelData myModelData;
	long long myVertexShaderID;
	long long myPixelShaderID;
	long long myGeometryShaderID;

	SBoundingBox myBoundingBox;
	std::string myDefaultAnimationID;
	bool myIsAnimated;
	bool myShouldCull;
};

