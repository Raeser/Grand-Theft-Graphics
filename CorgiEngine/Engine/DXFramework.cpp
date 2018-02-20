#include "stdafx.h"
#include "FullscreenTexture.h"
#include "PostMaster.h"
#include "..\Game\GameSettings.h"

CDXFramework::CDXFramework()
{
	myDevice = nullptr;
	myContext = nullptr;
}


CDXFramework::~CDXFramework()
{
#ifndef _RETAIL
	ID3D11Debug* debug;
	myDevice->QueryInterface(IID_PPV_ARGS(&debug));
	debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif

	myDevice->Release();
	myContext->Release();
	mySwapchain->Release();

	myDevice = nullptr;
	myContext = nullptr;
	mySwapchain = nullptr;
	CPostMaster::GetInstance().Unsubscribe(this, eMessageType::FullscreenData);
	CPostMaster::GetInstance().Unsubscribe(this, eMessageType::SetResolution);
}

bool CDXFramework::Init(CWindowHandler & aWindowHandler)
{
	myVSyncEnabled = false;
	CPostMaster::GetInstance().Subscribe(this, eMessageType::FullscreenData);
	CPostMaster::GetInstance().Subscribe(this, eMessageType::SetResolution);

	HRESULT result;
	DXGI_SWAP_CHAIN_DESC swapchainDescription = {};
	swapchainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDescription.BufferCount = 1;
	swapchainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDescription.OutputWindow = aWindowHandler.GetWindowHandle();
	swapchainDescription.SampleDesc.Count = 1;
	swapchainDescription.Windowed = true;

	swapchainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapchainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapchainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapchainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT creationFlags = 0;
	D3D_FEATURE_LEVEL level[2] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1

	};
#ifndef _RETAIL
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	result = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, creationFlags, level, 2, D3D11_SDK_VERSION,
		&swapchainDescription, &mySwapchain, &myDevice, nullptr, &myContext);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "Creation of Device and swapchain failed");
		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;

	ID3D11SamplerState* WrappingSampler = nullptr;
	HRESULT hr = myDevice->CreateSamplerState(&samplerDesc, &WrappingSampler);
	if (FAILED(hr))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "Creation of Wrapping sampler failed");
		return false;
	}

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	ID3D11SamplerState* ClampingSampler = nullptr;
	hr = myDevice->CreateSamplerState(&samplerDesc, &ClampingSampler);
	if (FAILED(hr))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "Creation of Clamping sampler failed");
		return false;
	}
	myContext->PSSetSamplers(0, 1, &ClampingSampler);
	myContext->PSSetSamplers(1, 1, &WrappingSampler);
	//D3D11_RASTERIZER_DESC desc;
	//desc.FillMode = D3D11_FILL_SOLID;
	//desc.CullMode = D3D11_CULL_FRONT;
	//desc.FrontCounterClockwise = FALSE;
	//desc.DepthBias = 0;
	//desc.DepthBiasClamp = 0.0f;
	//desc.SlopeScaledDepthBias = 0.0f;
	//desc.DepthClipEnable = TRUE;
	//desc.ScissorEnable = FALSE;
	//desc.MultisampleEnable = FALSE;
	//desc.AntialiasedLineEnable = FALSE;
	//
	//
	//ID3D11RasterizerState* rasterstate;
	//result = myDevice->CreateRasterizerState(&desc, &rasterstate);
	//result;
	//
	//myContext->RSSetState(rasterstate);

	return true;
}

bool CDXFramework::SetBackBuffer(CFullscreenTexture& aFullscreenTexture)
{
	HRESULT result;
	ID3D11Texture2D* backBufferTexture;
	result = mySwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "failed to get swapchainbuffer");
		return false;
	}
	aFullscreenTexture.Init(backBufferTexture);
	return true;
}


void CDXFramework::EndFrame()
{	
	//Using a bool to represent on or off (0 = off , 1 = on)
	mySwapchain->Present(myVSyncEnabled, 0);
}

void CDXFramework::SetFullscreen(bool aIsFullscreen)
{
	mySwapchain->SetFullscreenState(aIsFullscreen, nullptr);
}

void CDXFramework::SetResolution(const Corgi::Vector2<int>& aResolution)
{
	DXGI_MODE_DESC desc;
	desc.Width = aResolution.x;
	desc.Height = aResolution.y;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.RefreshRate = { 60, 1 };
	desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	mySwapchain->ResizeTarget(&desc);
	CGameSettings::GetInstance().UpdateResolutions();

}

void CDXFramework::RecieveMessage(const SMessage & message)
{
	switch (message.messageType)
	{
	case eMessageType::FullscreenData:
		SetFullscreen(message.fullscreenData.shouldFullscreen);
		break;
	case eMessageType::SetResolution:
		SetResolution(message.myResolutionData.myResolution);
		break;
	default:
		break;
	}
}

ID3D11DeviceContext * CDXFramework::GetContext()
{
	return myContext;
}

ID3D11Device * CDXFramework::GetDevice()
{
	return myDevice;
}
IDXGISwapChain * CDXFramework::GetSwapchain()
{
	return mySwapchain;
}


ID3D11Texture2D * CDXFramework::GetLastFrame()
{
	return myLastFrame;
}

void CDXFramework::SetIsVsyncEnabled(bool aIsEnabled)
{
	myVSyncEnabled = aIsEnabled;
}

bool CDXFramework::GetIsVsyncEnabled()
{
	return myVSyncEnabled;
}
