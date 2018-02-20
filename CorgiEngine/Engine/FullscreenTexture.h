#pragma once
#include "dxgiformat.h"

class CFullscreenTexture
{
public:
	CFullscreenTexture();
	~CFullscreenTexture();

	bool Init(Corgi::Vector2<int> aSize, DXGI_FORMAT aFormat, bool aCreateDepth = false);
	bool Init(ID3D11Texture2D* aTexture, bool aCreateDepth = false);

	void ClearTexture(float aClearColor[4]);
	void SetAsActiveTarget();
	void SetAsActiveTarget(ID3D11DepthStencilView* aViewToUse);
	void SetAsResourceOnSlot(unsigned int aSlot);
	void SetShaderResourceView(ID3D11ShaderResourceView* aResource);
	ID3D11RenderTargetView* GetRenderTarget();
	ID3D11Texture2D* GetTexture();
	ID3D11ShaderResourceView * GetShaderResource();
	ID3D11DepthStencilView* GetDepthStencilView();

	D3D11_VIEWPORT* GetViewport();

	static void AssignFramework(CDXFramework* aFramework);
	const Vector2int& GetSize();

private:
	Vector2int mySize;
	static CDXFramework* myFramework;
	ID3D11Texture2D* myTexture;
	ID3D11Texture2D* myBackBuffer;
	ID3D11ShaderResourceView* myShaderResource;
	ID3D11RenderTargetView* myRenderTarget;
	ID3D11DepthStencilView* myDepth;
	D3D11_VIEWPORT* myViewport;
	
};

