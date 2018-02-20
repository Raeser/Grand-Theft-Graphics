#include "stdafx.h"
#include "FullscreenRenderer.h"
#include "FullscreenTexture.h"
#include "ShaderManager.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "..\DirectX\Inc\DDSTextureLoader.h"
#include "..\Utilities\FileWatcher\FileWatcher.h"



CFullscreenRenderer::CFullscreenRenderer(CShaderManager& aShaderManager) :
	myShaderManager(aShaderManager)
{
}


CFullscreenRenderer::~CFullscreenRenderer()
{	
}

bool CFullscreenRenderer::Init(CDXFramework * aFrameWork)
{
	myFrameWork = aFrameWork;
	LoadVertexData();
	AddShader("../Assets/Shaders/PS_FullscreenCopy.hlsl", EEffect_Copy);
	AddShader("../Assets/Shaders/PS_BloomAdd.hlsl", EEffect_BloomAdd);
	AddShader("../Assets/Shaders/PS_BloomLuminance.hlsl", EEffect_BloomLuminance);
	AddShader("../Assets/Shaders/PS_GaussianBlurHorizontal.hlsl", EEffect_GaussianBlurHor);
	AddShader("../Assets/Shaders/PS_GaussianBlurVertical.hlsl", EEffect_GaussianBlurVer);
	AddShader("../Assets/Shaders/PS_FullscreenAdd.hlsl", EEffect_CombineTextures);
	AddShader("../Assets/Shaders/PS_ColorGrading.hlsl", EEffect_ColorGrading);
	AddShader("../Assets/Shaders/PS_Tonemap.hlsl", EEffect_Tonemap);

	LoadColorGradingTexture();
	Corgi::CFileWatcher::GetInstance().WatchFileChange(Corgi::ConvertCharPtrToWideString("../Assets/LookUpTables/RBGLUT_ColorCorrection.dds"), std::bind([&]() {LoadColorGradingTexture(); }));

	return true;
}

void CFullscreenRenderer::LoadVertexData()
{
	HRESULT result;




	//Create triangle vertices
	Vertex vertices[4] =
	{
		{ 1.0f, 1.0f, 0.0f,	1.f,		1.f,0.f},
		{ -1.0f, 1.0f, 0.0f, 1.f,		0.f,0.f },
		{ 1.0f, -1.0f, 0.0f, 1.f,		1.f,1.f },
		{ -1.0f, -1.0f, 0.0f, 1.f,		0.f,1.f }
	};
	unsigned int indices[6] =
	{
		0,2,1,
		3,1,2
	};

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.ByteWidth = sizeof(vertices);
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA bufferSubResourceData = { 0 };
	bufferSubResourceData.pSysMem = vertices;

	ID3D11Buffer* vertexBuffer;
	result = myFrameWork->GetDevice()->CreateBuffer(&bufferDescription, &bufferSubResourceData, &vertexBuffer);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "VertexBuffer failed to load");
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Render, "%s", "VertexBuffer load succeded");
	}
	D3D11_BUFFER_DESC indexBufferDescription = {};
	indexBufferDescription.ByteWidth = sizeof(indices);
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferSubResourceData = { 0 };
	indexBufferSubResourceData.pSysMem = indices;

	ID3D11Buffer* indexBuffer;
	result = myFrameWork->GetDevice()->CreateBuffer(&indexBufferDescription, &indexBufferSubResourceData, &indexBuffer);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "IndexBuffer failed to load");
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Render, "%s", "IndexBuffer load succeded");
	}
	//Create Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0 , DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	myVertexShaderID = myShaderManager.LoadShader("../Assets/Shaders/VS_Fullscreen.hlsl", layout, 2);



	myVertexData.myNumberOfVertices = sizeof(vertices) / sizeof(Vertex);
	myVertexData.myNumberOfIndices = sizeof(indices) / sizeof(UINT);
	myVertexData.myStride = sizeof(Vertex);
	myVertexData.myOffset = 0;
	myVertexData.myVertexBuffer = vertexBuffer;
	myVertexData.myIndexBuffer = indexBuffer;

}

void CFullscreenRenderer::AddShader(const char * aPath, EEffect aEffect)
{	
	myPixelShaders[aEffect] = myShaderManager.LoadShader(aPath, nullptr, 0);
}

void CFullscreenRenderer::Render(EEffect aEffect, std::array<CFullscreenTexture*, 2> aFullscreenTexture)
{
	aFullscreenTexture[0]->SetAsResourceOnSlot(0);
	if (aFullscreenTexture[1] != nullptr)
	{
		aFullscreenTexture[1]->SetAsResourceOnSlot(1);
	}

	ID3D11DeviceContext* context = myFrameWork->GetContext();

	if (aEffect == EEffect_ColorGrading)
	{
		context->PSSetShaderResources(2, 1, &myColorGradingLookUpTable);
	}
	else
	{
		ID3D11ShaderResourceView* clearVolume = nullptr;
		context->PSSetShaderResources(2, 1, &clearVolume);
	}

	const CVertexShader& fullscreenVertexShader = myShaderManager.GetVertexShader(myVertexShaderID);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(fullscreenVertexShader.GetInputLayout());

	context->IASetVertexBuffers(0, 1, &myVertexData.myVertexBuffer, &myVertexData.myStride, &myVertexData.myOffset);
	context->IASetIndexBuffer(myVertexData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->VSSetShader(fullscreenVertexShader.GetShader(), nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(myShaderManager.GetPixelShader(myPixelShaders[aEffect]).GetShader(), nullptr, 0);
	context->DrawIndexed(myVertexData.myNumberOfIndices, 0, 0);

	ID3D11ShaderResourceView* clear = nullptr;
	ID3D11ShaderResourceView* clear2 = nullptr;

	context->PSSetShaderResources(0, 1, &clear);
	context->PSSetShaderResources(1, 1, &clear2);

}



void CFullscreenRenderer::LoadColorGradingTexture()
{
	std::string volumepath = "../Assets/LookUpTables/RBGLUT_ColorCorrection.dds";
	myColorGradingLookUpTable = LoadTexture(volumepath);
}

ID3D11ShaderResourceView* CFullscreenRenderer::LoadTexture(std::string& aFilePath)
{

	std::wstring filename;
	filename.assign(aFilePath.begin(), aFilePath.end());
	ID3D11ShaderResourceView * creation;
	HRESULT result = DirectX::CreateDDSTextureFromFile(myFrameWork->GetDevice(), filename.c_str(), nullptr, &creation);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s%s", "[ModelLoader] - LoadTexture failed to load file: ", aFilePath.c_str());
		if (result == D3D11_ERROR_FILE_NOT_FOUND)
		{
			DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - Texture File was not found");
		}
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s%s%s", "[ModelLoader] - Texture ", aFilePath.c_str(), " load succeded");
	}
	return creation;
}