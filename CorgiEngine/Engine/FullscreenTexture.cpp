#include "stdafx.h"
#include "FullscreenTexture.h"

CDXFramework* CFullscreenTexture::myFramework = nullptr;


CFullscreenTexture::CFullscreenTexture()
{
	myDepth = nullptr;
	myFramework = nullptr;
	myTexture = nullptr;
}


CFullscreenTexture::~CFullscreenTexture()
{
	myTexture->Release();
	myTexture = nullptr;
	if (myShaderResource != nullptr)
	{
		myShaderResource->Release();
		myShaderResource = nullptr;
	}
	if (myBackBuffer != nullptr)
	{
		myBackBuffer->Release();
		myBackBuffer = nullptr;
	}
	myRenderTarget->Release();
	myRenderTarget = nullptr;

	if (myDepth != nullptr)
	{
		myDepth->Release();
		myDepth = nullptr;
	}
}

bool CFullscreenTexture::Init(Corgi::Vector2<int> aSize, DXGI_FORMAT aFormat, bool aCreateDepth)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = static_cast<UINT>(aSize.x);
	textureDesc.Height = static_cast<UINT>(aSize.y);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = aFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	HRESULT result;
	result = myFramework->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &myTexture);
	if (FAILED(result))
	{
		DL_WARNING(eDebugLogType::Resource, "%s", "FullScreenTexture failed to initiate texture");
		return false;
	}

	result = myFramework->GetDevice()->CreateRenderTargetView(myTexture, nullptr, &myRenderTarget);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "failed to create Render Target View");
		return false;
	}

	result = myFramework->GetDevice()->CreateShaderResourceView(myTexture, nullptr, &myShaderResource);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s%s", "FullScreenTexture: ", "failed to create ShaderResource View");
		return false;
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
			DL_ERROR(eDebugLogType::Render, "%s", "failed to create BackDepthTexture");
			return false;
		}

		result = myFramework->GetDevice()->CreateDepthStencilView(backdepthTexture, nullptr, &myDepth);

		if (FAILED(result))
		{
			DL_ERROR(eDebugLogType::Render, "%s", "Failed to create Depth Stencil View");
			return false;
		}

		backdepthTexture->Release();

	}
	mySize = aSize;
	return true;
}

bool CFullscreenTexture::Init(ID3D11Texture2D * aTexture, bool aCreateDepth)
{
	myTexture = aTexture;

	D3D11_TEXTURE2D_DESC desc;
	aTexture->GetDesc(&desc);

	HRESULT result;
	result = myFramework->GetDevice()->CreateRenderTargetView(myTexture, nullptr, &myRenderTarget);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "failed to create Render Target View");
		return false;
	}


	myViewport = new D3D11_VIEWPORT();

	myViewport->TopLeftX = 0.0f;
	myViewport->TopLeftY = 0.0f;
	myViewport->Width = static_cast<float>(desc.Width);
	myViewport->Height = static_cast<float>(desc.Height);
	myViewport->MinDepth = 0.0f;
	myViewport->MaxDepth = 1.0f;

	if (aCreateDepth)
	{
		ID3D11Texture2D* backdepthTexture;
		D3D11_TEXTURE2D_DESC backdepthTextureDescription = {};
		backdepthTextureDescription.Width = desc.Width;
		backdepthTextureDescription.Height = desc.Height;
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

	mySize = { static_cast<int>(desc.Width), static_cast<int>(desc.Height) };

	return true;
}

void CFullscreenTexture::ClearTexture(float aClearColor[4])
{
	myFramework->GetContext()->ClearRenderTargetView(myRenderTarget, aClearColor);
	if (myDepth)
	{
		myFramework->GetContext()->ClearDepthStencilView(myDepth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}
}

void CFullscreenTexture::SetAsActiveTarget()
{
	myFramework->GetContext()->RSSetViewports(1, myViewport);
	myFramework->GetContext()->OMSetRenderTargets(1, &myRenderTarget, myDepth);
}

void CFullscreenTexture::SetAsActiveTarget(ID3D11DepthStencilView* aViewToUse)
{
	myFramework->GetContext()->RSSetViewports(1, myViewport);
	myFramework->GetContext()->OMSetRenderTargets(1, &myRenderTarget, aViewToUse);
}

void CFullscreenTexture::SetAsResourceOnSlot(unsigned int aSlot)
{
	myFramework->GetContext()->PSSetShaderResources(aSlot, 1, &myShaderResource);
}

void CFullscreenTexture::SetShaderResourceView(ID3D11ShaderResourceView * aResource)
{
	myShaderResource = aResource;
}

ID3D11RenderTargetView * CFullscreenTexture::GetRenderTarget()
{
	return myRenderTarget;
}

ID3D11Texture2D * CFullscreenTexture::GetTexture()
{
	return myTexture;
}

ID3D11ShaderResourceView* CFullscreenTexture::GetShaderResource()
{
	return myShaderResource;
}

ID3D11DepthStencilView * CFullscreenTexture::GetDepthStencilView()
{
	return myDepth;
}

D3D11_VIEWPORT * CFullscreenTexture::GetViewport()
{
	return myViewport;
}

void CFullscreenTexture::AssignFramework(CDXFramework * aFramework)
{
	myFramework = aFramework;
}

const Vector2int & CFullscreenTexture::GetSize()
{
	return mySize;
}
