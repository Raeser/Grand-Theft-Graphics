#pragma once
#include "..\Utilities\simple_pointer_array.h"
#include "..\Utilities\Vector.h"
#include "ShaderCompiler.h"

using TextID = long long;
using FontID = long long;

struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D11Buffer;

using UINT = unsigned;

enum eTextType
{
	eText_Normal,
	eText_Scrolling
};

struct STextToRender
{
	Vector2f mySize;
	Vector2f myUV;
	Vector2f uvScale;
	Vector2f myPosition;
	Vector4f myColor;
};

struct SSimplifiedText
{
	std::string* text;
	std::string bufferedText;
	Vector4f myScreenPosition;
	Vector4f color;
	FontID fontID;
	int scrollingIndex;
	float scale;
	eTextType textType;
	bool isActive;
};

class CFontData;

class CTextRenderer
{
public:

	CTextRenderer(CEngine& aEngine);
	~CTextRenderer();

	void Init();
	void Render(SSimplifiedText& aTextToRender, CFontData& aFontToUse);

private:
	void BuildText(const std::string& aText, Vector4f aColor, float aSize, CFontData& aDataPtr);
	bool LoadTextShaders();
	bool CreateTextVertexBuffer();
	bool CreateTextInstanceBuffer();
	bool CreateTextInputLayout(ID3D10Blob* aBlob);
	bool CreateViewPositionBuffer();

	bool UpdateTextBuffers();
	bool UpdateInstanceBuffer(const Vector4f& aPosition);


	Corgi::GrowingArray<STextToRender, size_t> myBufferedObjects;
	CEngine& myEngine;
	int myCurrentCharacterAmount;
	ID3D11Buffer* myTextVertexBuffer;
	ID3D11Buffer* myTextInstanceBuffer;
	UINT myStride;
	UINT myOffset;
	ID3D11InputLayout* myTextInputLayout;
	ID3D11VertexShader* myTextVertexShader;
	ID3D11PixelShader* myTextPixelShader;
	ID3D11GeometryShader* myTextGeometryShader;

	CShaderCompiler myShaderCompiler;


};

