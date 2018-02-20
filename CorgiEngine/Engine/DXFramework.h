#pragma once
#include "Subscriber.h"

class CWindowHandler;
class CFullscreenTexture;
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct D3D11_VIEWPORT;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

class CDXFramework : public CSubscriber
{
public:
	CDXFramework();
	~CDXFramework();

	bool Init(CWindowHandler& aWindowHandler);
	bool SetBackBuffer(CFullscreenTexture & aFullscreenTexture);

	void EndFrame();

	void SetFullscreen(bool aIsFullscreen);

	void RecieveMessage(const SMessage& message) override;

	void SetResolution(const Corgi::Vector2<int>& aResolution);

	ID3D11DeviceContext* GetContext();
	ID3D11Device* GetDevice();
	IDXGISwapChain* GetSwapchain();
	ID3D11Texture2D* GetLastFrame();
	void SetIsVsyncEnabled(bool aIsEnabled);
	bool GetIsVsyncEnabled();

	void SetClampingSampleState();
	void SetWrappingSampleState();

private:
	IDXGISwapChain* mySwapchain;
	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;
	ID3D11Texture2D* myLastFrame;


	bool myVSyncEnabled;
};

