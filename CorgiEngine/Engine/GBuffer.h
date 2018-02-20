#pragma once
#include "dxgiformat.h"
#include "..\Utilities\Vector.h"
#include <wtypes.h>


enum eTargetType
{
	eTarget_WorldPosition,
	eTarget_Albedo,
	eTarget_Normals,
	eTarget_Metallic,
	eTarget_Roughness,
	eTarget_EmissiveAndSpec,
	Targetcount
};

struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;
struct D3D11_VIEWPORT;
class CDXFramework;

class CGBuffer
{
public:
	CGBuffer();
	~CGBuffer();

	bool Init(Corgi::Vector2<int> aSize, DXGI_FORMAT aWorldPosFormat, DXGI_FORMAT aFormat, UINT aNumOfTextures, bool aCreateDepth = false);

	void ClearTexture(float aClearColor[4]);
	void SetAsActiveTarget();
	void SetAsActiveTarget(eTargetType aTarget);
	void SetAsResourceOnSlot(unsigned int aSlot);
	void SetAsResourceOnSlot(unsigned int aSlot, eTargetType aTarget);

	ID3D11RenderTargetView* GetRenderTarget();
	ID3D11RenderTargetView* GetRenderTarget(eTargetType aTarget);
	ID3D11Texture2D* GetTexture() const;
	static void AssignFramework(CDXFramework* aFramework);
	ID3D11DepthStencilView* GetDepthStencilView();

private:
	static CDXFramework* myFramework;
	ID3D11Texture2D** myTextures;
	ID3D11Texture2D* myBackBuffer;
	ID3D11ShaderResourceView** myShaderResources;
	ID3D11RenderTargetView** myRenderTargets;
	ID3D11DepthStencilView* myDepth;
	D3D11_VIEWPORT* myViewport;

	UINT myNumberOfViews;
};
