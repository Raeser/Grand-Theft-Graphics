/*
This class handles all texts that will be rendered, don't use this to show text, only use the CText class
*/

#pragma once

#define FONT_BUFF_SIZE 4096
#define MAX_SPRITES 10000

#include "fontfile.h"
#include "../Utilities/simple_pointer_array.h"
#include <unordered_map>




class ID3D11ShaderResourceView;
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

struct STextToRender
{
	Vector2f mySize;
	Vector2f myUV;
	Vector2f uvScale;
	Vector2f myPosition;
	Vector4f myColor;
};
class CFontData
{
public:
	CFontData()
	{
		myRenderCountChanged = false;
		myLastRenderCount = 0;
		myBufferedObjects.Init(FONT_BUFF_SIZE + 1);
		for (int i = 0; i < myBufferedObjects.TotalCount(); i++)
		{
			STextToRender *text = new STextToRender();
			myBufferedObjects[i] = text;
		}
		for (int i=0; i< 1024; i++)
		{
			myCharData.push_back(SCharData());
		}
	}
	~CFontData();
	//std::unordered_map<char, SCharData> myCharData;
	std::vector<SCharData> myCharData;
	unsigned int myAtlasWidth;
	unsigned int myAtlasHeight;
	float myLineSpacing;
	unsigned int myWordSpacing;
	unsigned int myFontHeightWidth;
	int* myAtlas;
	std::string myName;
	CSpriteBatch* myBatch;
	Corgi::SSimplePointerArray<STextToRender*> myBufferedObjects;
	int myLastRenderCount;
	bool myRenderCountChanged;
	ID3D11ShaderResourceView* myAtlasView;
};


struct TextVertexNew
{
	TextVertexNew() { myPosition = { 0, 0 }; myTexCoord = { 0, 0 }; }
	TextVertexNew(Vector2f aPos, Vector2f aTexCoord, Vector4f color = Vector4f(1, 1, 1, 1))
	{
		myPosition = aPos;
		myTexCoord = aTexCoord;
		myColor = color;
	}
	Vector2f myPosition;
	Vector2f myTexCoord;
	Vector4f myColor;
};

class CSpriteBatch;
class CTexture;
class CSprite;
class CText;
class CTextService
{
	struct SText
	{
		std::string myText;
		std::string myName;
		Vector2f myPosition;
		float mySize;
		Vector4f myColor;
	};
public:
	CTextService();
	~CTextService();

	void Init();
	
	//void Render();

	void Update();

	CFontData* InitAndLoad(std::string aFontPathAndName, unsigned char aBorderSize, int aSize);
	bool AddTextToRender(const std::string& aText, const Vector2f& aPosition, Vector4f aColor, float aSize, CFontData* aDataPtr);
	float GetSentenceWidth(CText* aText);
	std::string GetRenderName(const std::string& aFontPathAndName, enum EFontSize aSize);

	void ClearMemory(const std::string& aRenderName);
private:
	bool UpdateTextAndDraw(const std::string& aText, const Vector2f& aPosition, Vector4f aColor, float aSize, CFontData* aDataPtr);

	void BuildText(const std::string& aText, Vector4f aColor, float aSize, CFontData* aDataPtr, Corgi::SSimplePointerArray<STextToRender*>& aTextBuffer);
	void InitTexture();
	
	float minS;
	float minT;
	float maxS;
	float maxT;
	int currentFontChar;
	int myCurrentSprite;
	unsigned int myTextsToDraw;

	Corgi::SSimplePointerArray<SText*> myTextsToRender;
	CSprite* mySprites;
	struct FT_LibraryRec_* myLibrary;

	void LoadGlyph(int index, int& atlasX, int& atlasY, int& maxY , float atlasWidth, float atlasHeight, CFontData* aFontData, struct FT_FaceRec_* aFace, int aBorderWidth = 0);
	void LoadOutline(const int index, const int atlasX, const int atlasY , const float atlasWidth, CFontData* aFontData, struct FT_FaceRec_* aFace, int aBorderWidth);
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

	std::unordered_map<std::string, CFontData*> myFontData;

	
};
