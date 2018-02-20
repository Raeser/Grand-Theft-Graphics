#pragma once
#include "D3D11DataWrappers.h"

#include "ShaderCompiler.h"
#include <array>

class CFullscreenTexture;
class CFullscreenRenderer
{
public:
	enum EEffect
	{
		EEffect_Copy,
		EEffect_BloomAdd,
		EEffect_BloomLuminance,
		EEffect_GaussianBlurHor,
		EEffect_GaussianBlurVer,
		EEffect_CombineTextures,
		EEffect_ColorGrading,
		EEffect_Tonemap,
		EEffect_Count
	};

public:
	CFullscreenRenderer(CShaderManager& aShaderManager);
	~CFullscreenRenderer();

	bool Init(CDXFramework* aFrameWork);
	void LoadVertexData();
	void AddShader(const char* aPath, EEffect aEffect);
	void Render(EEffect aEffect, std::array<CFullscreenTexture*, 2> aFullscreenTexture);

private:

	struct SFullscreenVertex
	{
		Vector4f myPosition;
		Vector2f myUV;
	};

private:

	void LoadColorGradingTexture();

	ID3D11ShaderResourceView * LoadTexture(std::string & aFilePath);

	CShaderManager& myShaderManager;
	CDXFramework* myFrameWork;
	long long myVertexShaderID;

	ID3D11ShaderResourceView* myColorGradingLookUpTable;

	SVertexDataWrapper myVertexData;
	std::array<long long, EEffect_Count> myPixelShaders;
};

