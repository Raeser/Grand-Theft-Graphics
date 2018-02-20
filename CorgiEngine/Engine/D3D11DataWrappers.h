#pragma once
#include <d3dcommon.h>
struct ID3D11ShaderResourceView;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;


struct SParticleVertexDataWrapper
{
	UINT myNumberOfVertices;
	UINT myStride;
	UINT myOffset;
	ID3D11Buffer* myVertexBuffer;
};

struct SVertexDataWrapper
{
	UINT myNumberOfVertices;
	UINT myNumberOfIndices;
	UINT myStride;
	UINT myOffset;
	ID3D11Buffer* myVertexBuffer;
	ID3D11Buffer* myIndexBuffer;

};

struct SShaderDataWrapper
{
	ID3D11VertexShader* myVertexShader;
	ID3D11PixelShader* myPixelShader;
	ID3D11GeometryShader* myGeometryShader;
};

struct SLayoutDataWrapper
{
	D3D_PRIMITIVE_TOPOLOGY myPrimitiveTopology;
	ID3D11InputLayout* myInputLayout;
};

struct SParticleDataWrapper
{
	float myLifeTime;
	float myTime;
	float myDistToEmit;
	Corgi::Vector3<float> myPosition;
	Corgi::Vector3<float> myVelocity;
	Corgi::Vector3<float> myTrash;
};

struct STextureDataWrapper
{
public:
	~STextureDataWrapper() {}

public:
	ID3D11ShaderResourceView* myAlbedo;
	ID3D11ShaderResourceView* myNormalMap;
	ID3D11ShaderResourceView* myRoughnessMap;
	ID3D11ShaderResourceView* myAmbientMap;
	ID3D11ShaderResourceView* myEmissiveMap;
	ID3D11ShaderResourceView* myMetalnessMap;
};

struct SParticleTextureDataWrapper
{
	ID3D11ShaderResourceView* myTexture;
};