#include "stdafx.h"
#include "GBuffer.h"

CDXFramework* CGBuffer::myFramework = nullptr;


CGBuffer::CGBuffer()
{
	myDepth = nullptr;
	myFramework = nullptr;
	//myTextures.Init(8);
}


CGBuffer::~CGBuffer()
{
	for (UINT i = 0; i < myNumberOfViews; i++)
	{
		myTextures[i]->Release();
		myTextures[i] = nullptr;
		myShaderResources[i]->Release();
		myShaderResources[i] = nullptr;
		myRenderTargets[i]->Release();
		myRenderTargets[i] = nullptr;
	}
	if (myBackBuffer != nullptr)
	{
		myBackBuffer->Release();
		myBackBuffer = nullptr;
	}
	myDepth->Release();
	myDepth = nullptr;
}

bool CGBuffer::Init(Corgi::Vector2<int> aSize, DXGI_FORMAT aWorldPosFormat, DXGI_FORMAT /*aFormat*/, UINT aNumOfTextures, bool aCreateDepth)
{
	myRenderTargets = new ID3D11RenderTargetView*[aNumOfTextures];
	myTextures = new ID3D11Texture2D*[aNumOfTextures];
	myShaderResources = new ID3D11ShaderResourceView*[aNumOfTextures];
	myNumberOfViews = aNumOfTextures;
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = static_cast<UINT>(aSize.x);
	textureDesc.Height = static_cast<UINT>(aSize.y);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = aWorldPosFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	HRESULT result;
	eTargetType type;
	for (UINT i = 0; i < myNumberOfViews; ++i)
	{
		type = static_cast<eTargetType>(i);
		switch (type)
		{
		case eTarget_WorldPosition:
			textureDesc.Format = aWorldPosFormat;
			break;
		case eTarget_Normals:
			textureDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
			break;
		case eTarget_Albedo:
		case eTarget_Metallic:
		case eTarget_Roughness:
		case eTarget_EmissiveAndSpec:
		default:
			textureDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
			break;
		}
		result = myFramework->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &myTextures[i]);
		if (FAILED(result))
		{
			DL_WARNING(eDebugLogType::Engine, "%s", "FullScreenTexture failed to initiate texture");
			return false;
		}

		result = myFramework->GetDevice()->CreateRenderTargetView(myTextures[i], nullptr, &myRenderTargets[i]);

		if (FAILED(result))
		{
			DL_ERROR(eDebugLogType::Engine, "%s", "failed to create Render Target View");
			return false;
		}

		result = myFramework->GetDevice()->CreateShaderResourceView(myTextures[i], nullptr, &myShaderResources[i]);
		if (FAILED(result))
		{
			DL_ERROR(eDebugLogType::Engine, "%s%s", "FullScreenTexture: ", "failed to create ShaderResource View");
			return false;
		}
	}

	myViewport = new D3D11_VIEWPORT();

	myViewport->TopLeftX = 0.0f;
	myViewport->TopLeftY = 0.0f;
	myViewport->Width = static_cast<float>(aSize.x);
	myViewport->Height = static_cast<float>(aSize.y);
	myViewport->MinDepth = 0.0f;
	myViewport->MaxDepth = 1.0f;

	if (aCreateDepth)
	{
		ID3D11Texture2D* backdepthTexture;
		D3D11_TEXTURE2D_DESC backdepthTextureDescription = {};
		backdepthTextureDescription.Width = static_cast<unsigned int>(aSize.x);
		backdepthTextureDescription.Height = static_cast<unsigned int>(aSize.y);
		backdepthTextureDescription.ArraySize = 1;
		backdepthTextureDescription.Format = DXGI_FORMAT_D32_FLOAT;
		backdepthTextureDescription.SampleDesc.Count = 1;
		backdepthTextureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		result = myFramework->GetDevice()->CreateTexture2D(&backdepthTextureDescription, nullptr, &backdepthTexture);

		if (FAILED(result))
		{
			DL_ERROR(eDebugLogType::Engine, "%s", "failed to create BackDepthTexture");
			return false;
		}

		result = myFramework->GetDevice()->CreateDepthStencilView(backdepthTexture, nullptr, &myDepth);

		if (FAILED(result))
		{
			DL_ERROR(eDebugLogType::Engine, "%s", "Failed to create Depth Stencil View");
			return false;
		}

		backdepthTexture->Release();

	}

	return true;
}

void CGBuffer::ClearTexture(float aClearColor[4])
{
	for (UINT i = 0; i < myNumberOfViews; ++i)
	{
		myFramework->GetContext()->ClearRenderTargetView(myRenderTargets[i], aClearColor);
	}
	if (myDepth)
	{
		myFramework->GetContext()->ClearDepthStencilView(myDepth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}
}

void CGBuffer::SetAsActiveTarget()
{
	myFramework->GetContext()->RSSetViewports(1, myViewport);
	myFramework->GetContext()->OMSetRenderTargets(myNumberOfViews, myRenderTargets, myDepth);
}

void CGBuffer::SetAsActiveTarget(eTargetType aTarget)
{
	myFramework->GetContext()->RSSetViewports(1, myViewport);
	myFramework->GetContext()->OMSetRenderTargets(1, &myRenderTargets[aTarget], myDepth);
}

void CGBuffer::SetAsResourceOnSlot(unsigned int aSlot)
{
	myFramework->GetContext()->PSSetShaderResources(aSlot, myNumberOfViews, myShaderResources);
}

void CGBuffer::SetAsResourceOnSlot(unsigned int aSlot, eTargetType aTarget)
{
	myFramework->GetContext()->PSSetShaderResources(aSlot, 1, &myShaderResources[aTarget]);
}

ID3D11RenderTargetView * CGBuffer::GetRenderTarget()
{
	return *myRenderTargets;
}

ID3D11RenderTargetView * CGBuffer::GetRenderTarget(eTargetType aTarget)
{
	return myRenderTargets[aTarget];
}

ID3D11Texture2D * CGBuffer::GetTexture() const
{
	return *myTextures;
}

void CGBuffer::AssignFramework(CDXFramework * aFramework)
{
	myFramework = aFramework;
}

ID3D11DepthStencilView * CGBuffer::GetDepthStencilView()
{
	return myDepth;
}
