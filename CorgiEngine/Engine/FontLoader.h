#pragma once

#include "..\Utilities\simple_pointer_array.h"

#include "..\Utilities\GrowingArray.h"
#include "..\Utilities\Vector2.h"

struct ID3D11ShaderResourceView;
class CEngine;
using FontID = long long;

enum EFontSize
{
	EFontSize_6 = 6,
	EFontSize_8 = 8,
	EFontSize_9 = 9,
	EFontSize_10 = 10,
	EFontSize_11 = 11,
	EFontSize_12 = 12,
	EFontSize_14 = 14,
	EFontSize_18 = 18,
	EFontSize_24 = 24,
	EFontSize_30 = 30,
	EFontSize_36 = 36,
	EFontSize_48 = 48,
	EFontSize_60 = 60,
	EFontSize_72 = 72,
	EFontSize_Count
};


struct SCharData
{
	Vector2f myTopLeftUV;
	Vector2f myBottomRightUV;

	short myWidth;
	short myHeight;
	short myAdvanceX; //Distance to next character.
	short myBearingX;
	short myBearingY;
	char myChar;
};

class CFontData
{
public:
	CFontData()
	{
		myRenderCountChanged = false;
		myLastRenderCount = 0;
		myCharData.Init(1024);
		for (int i = 0; i < 1024; i++)
		{
			myCharData.Add(SCharData());
		}
	}
	~CFontData();
	//std::unordered_map<char, SCharData> myCharData;
	Corgi::GrowingArray<SCharData> myCharData;
	unsigned int myAtlasWidth;
	unsigned int myAtlasHeight;
	float myLineSpacing;
	unsigned int myWordSpacing;
	unsigned int myFontHeightWidth;
	int* myAtlas;
	std::string myName;
	//CSpriteBatch* myBatch; //FIX
	//Corgi::SSimplePointerArray<STextToRender*> myBufferedObjects;
	int myLastRenderCount;
	bool myRenderCountChanged;
	ID3D11ShaderResourceView* myAtlasView;
};

class CFontLoader
{
public:
	CFontLoader(CEngine& aEngine);
	~CFontLoader();
	bool InitAndLoad(CFontData& aFontDataOut, std::string aFontPathAndName, unsigned char aBorderSize, int aSize);

private:
	struct FT_LibraryRec_* myLibrary;

	void LoadGlyph(int index, int& atlasX, int& atlasY, int& maxY, float atlasWidth, float atlasHeight, CFontData* aFontData, struct FT_FaceRec_* aFace, int aBorderWidth = 0);
	void LoadOutline(const int index, const int atlasX, const int atlasY, const float atlasWidth, CFontData* aFontData, struct FT_FaceRec_* aFace, int aBorderWidth);
	void CalculateOutlineOffsets(const int index, struct FT_FaceRec_* aFace, int aBorderWidth);
	void CalculateGlyphOffsets(const int index, struct FT_GlyphSlotRec_* glyph);

	struct SCountData
	{
		int xNCount = 0;
		int xPCount = 0;
		int yNCount = 0;
		int yPCount = 0;
	};

	void CountOffsets(const int& x, const int& y, const int& width, const int& height, SCountData& countData);
	void CountDeltas(const int& width, const int& height, int& deltaX, int& deltaY, SCountData& countData);
	struct SOutlineOffset
	{
		int xDelta = 0;
		int yDelta = 0;

	} myOffset;

	CEngine& myEngine;
};

