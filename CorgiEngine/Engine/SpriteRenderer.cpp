#include "stdafx.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "SpriteLoader.h"
#include "SpriteInstance.h"


CSpriteRenderer::CSpriteRenderer(CSpriteLoader & aSpriteLoader) :
	mySpriteLoader(aSpriteLoader)
{
}

CSpriteRenderer::~CSpriteRenderer()
{
	mySpriteBuffer->Release();
	mySpriteBuffer = nullptr;
}

bool CSpriteRenderer::Init(CDXFramework * aDXFramework)
{
	myDXFramework = aDXFramework;
	InitSpriteDataBuffer();
	return true;
}

bool CSpriteRenderer::InitSpriteDataBuffer()
{
	ID3D11Device* device = myDXFramework->GetDevice();
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.ByteWidth = sizeof(SSpriteStruct);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = device->CreateBuffer(&bufferDescription, nullptr, &mySpriteBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "ForwardRenderer failed to create ViewPositionBuffer");
		if (result == E_INVALIDARG)
		{
			DL_ERROR(eDebugLogType::Render, "%s", "invalidArgument !?");
		}
		return false;
	}
	return true;
}

CSpriteInstance * CSpriteRenderer::CreateInstance()
{
	CSpriteInstance* returnMe = new CSpriteInstance();
	returnMe->SetSprite(-1);
	return returnMe;
}

CSpriteInstance * CSpriteRenderer::CreateInstance(long long aSpriteID)
{

	CSpriteInstance* returnMe = new CSpriteInstance();
	if (aSpriteID == -1 || aSpriteID > mySpriteLoader.GetAmountOfSprites() - 1)
	{
		DL_WARNING(eDebugLogType::Resource, "%s%i%s", "You tried creating a Sprite with a non existing Id: ", (int)aSpriteID, " Defaulting To ErrorSprite!");
		aSpriteID = 0;
	}
	returnMe->SetSprite(aSpriteID);
	return returnMe;

}

CSpriteInstance * CSpriteRenderer::CreateInstance(const char * aSpritePath)
{
	CSpriteInstance* returnMe = new CSpriteInstance();
	returnMe->SetSprite(mySpriteLoader.CreateSprite(aSpritePath));
	return returnMe;
}

void CSpriteRenderer::Render(const Vector2int& aActiveTargetSize)
{
	ID3D11DeviceContext* context = myDXFramework->GetContext();
	for (CSpriteInstance& SpriteInstance : mySpriteLoader.GetCurrentInstances())
	{
		if (SpriteInstance.GetShouldRender() == false)
		{
			continue;
		}

		CSprite& sprite = mySpriteLoader.GetSprite(SpriteInstance.GetSpriteID());
		SVertexDataWrapper& vertexData = sprite.GetVertexData();
		const SShaderDataWrapper& shaderData = sprite.GetShaderData();
		const SLayoutDataWrapper& layoutData = sprite.GetLayoutData();
		const STextureDataWrapper& textureData = sprite.GetTextureData();

		UpdateSpriteBuffer(sprite.GetOriginalSize(), SpriteInstance.GetPosition(), SpriteInstance.GetColor(), SpriteInstance.GetUVOffset(), aActiveTargetSize);


		context->IASetPrimitiveTopology(layoutData.myPrimitiveTopology);
		context->IASetInputLayout(layoutData.myInputLayout);
		context->IASetVertexBuffers(0, 1, &vertexData.myVertexBuffer, &vertexData.myStride, &vertexData.myOffset);
		context->IASetIndexBuffer(vertexData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		context->VSSetShader(shaderData.myVertexShader, nullptr, 0);
		context->VSSetConstantBuffers(0, 1, &mySpriteBuffer);
		context->PSSetShaderResources(0, 1, &textureData.myAlbedo);
		context->PSSetShader(shaderData.myPixelShader, nullptr, 0);

		context->DrawIndexed(vertexData.myNumberOfIndices, 0, 0);
	}
}

void CSpriteRenderer::Render(CSpriteInstance * aSpriteInstance, const Vector2int& aActiveTargetSize)
{
	ID3D11DeviceContext* context = myDXFramework->GetContext();

	CSprite& sprite = mySpriteLoader.GetSprite(aSpriteInstance->GetSpriteID());
	SVertexDataWrapper& vertexData = sprite.GetVertexData();
	const SShaderDataWrapper& shaderData = sprite.GetShaderData();
	const SLayoutDataWrapper& layoutData = sprite.GetLayoutData();
	const STextureDataWrapper& textureData = sprite.GetTextureData();

	UpdateSpriteBuffer(sprite.GetOriginalSize(), aSpriteInstance->GetPosition(), aSpriteInstance->GetColor(), aSpriteInstance->GetUVOffset(), aActiveTargetSize);


	context->IASetPrimitiveTopology(layoutData.myPrimitiveTopology);
	context->IASetInputLayout(layoutData.myInputLayout);
	context->IASetVertexBuffers(0, 1, &vertexData.myVertexBuffer, &vertexData.myStride, &vertexData.myOffset);
	context->IASetIndexBuffer(vertexData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	context->VSSetShader(shaderData.myVertexShader, nullptr, 0);
	context->VSSetConstantBuffers(0, 1, &mySpriteBuffer);
	context->PSSetShaderResources(0, 1, &textureData.myAlbedo);
	context->PSSetShader(shaderData.myPixelShader, nullptr, 0);

	context->DrawIndexed(vertexData.myNumberOfIndices, 0, 0);

}

void CSpriteRenderer::AssignFramework(CDXFramework * aFramework)
{
	myDXFramework = aFramework;
}

bool CSpriteRenderer::UpdateSpriteBuffer(const Vector2f& aSpriteSize, const Vector3f & aPos, const Vector4f & aColor, const Vector2f & aUVOffset, const Vector2int& aActiveTargetSize)
{
	aSpriteSize;
	mySpriteData.Color = aColor;
	mySpriteData.Position = aPos;
	mySpriteData.TargetSize = { static_cast<float>(aActiveTargetSize.x), static_cast<float>(aActiveTargetSize.y) };
	mySpriteData.UVOffset = aUVOffset;
	ID3D11DeviceContext* context = myDXFramework->GetContext();

	D3D11_MAPPED_SUBRESOURCE data;
	HRESULT result;

	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = context->Map(mySpriteBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "sprite renderer failed to update spritebuffer");
		return false;
	}

	memcpy(data.pData, &mySpriteData, sizeof(mySpriteData));
	context->Unmap(mySpriteBuffer, 0);

	return true;
}

