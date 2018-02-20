#include "stdafx.h"

#include <ft2build.h>

#ifdef _DEBUG
#pragma comment (lib, "freetype_Debug_x64.lib")
#else
#pragma comment (lib, "freetype_Release_x64.lib")
#endif

#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H
#include FT_STROKER_H
#include FT_OUTLINE_H

#include "text_service.h"
//#include "textfile.h"
//#include "fontfile.h"
//#include "text.h"
#include "../engine.h"
#include <fstream>

#include "texture/texture_manager.h"				  //make em work bruh
#include "sprite/sprite_batch.h"					  //make em work bruh
#include "sprite/sprite.h"							  //make em work bruh
#include "sprite/textured_quad_batch.h"				  //make em work bruh


#define SAFE_RELEASE(aPointer) if(aPointer){ aPointer->Release();}
#define SAFE_DELETE(aPointer) delete aPointer; aPointer = nullptr;
#define X_OFFSET 8
#define X_START 2
#define Y_OFFSET 8


CFontData::~CFontData()
{
	SAFE_DELETE(myAtlas);
	SAFE_RELEASE(myAtlasView);

	delete myBatch;
}

CTextService::CTextService()
{
	minS = 0;
	minT = 0;
	maxS =0;
	maxT = 0;
	myCurrentSprite = 0;
	myTextsToDraw = 0;

}


CTextService::~CTextService()
{
	delete[]mySprites;

	for (std::unordered_map<std::string, CFontData*>::iterator iterator = myFontData.begin(); iterator != myFontData.end(); iterator++)
	{
		delete iterator->second;
	}
	myFontData.clear();

	for (int i = 0; i < myTextsToRender.TotalCount(); i++)
	{
		delete myTextsToRender.myData[i];
	}

}

void CTextService::Init()
{
	//ZeroMemory(mySprites, sizeof(mySprites));

	mySprites = new CSprite[MAX_SPRITES]();
	myTextsToRender.Init(128);

	for (int i = 0; i < 128; i++)
	{
		SText* text = new SText();
		myTextsToRender.myData[i] = text;
	}

	FT_Init_FreeType(&myLibrary);
}

inline bool FileExists(const std::string& name) 
{
	std::ifstream f(name.c_str());
	if (f.good()) 
	{
		f.close();
		return true;
	}

	f.close();
	return false;
}

CFontData* CTextService::InitAndLoad(std::string aFontPath, unsigned char aBorderSize, int aSize)
{
	if (aSize == -1)
	{
		return nullptr;
	}
	
	short aFontWidth = (short)aSize;
	int atlasSize = 1024;
	float atlasWidth = static_cast<float>(atlasSize);
	float atlasHeight = static_cast<float>(atlasSize);

	int atlasX = X_START;
	int atlasY = 2;
	int currentMaxY = 0;

	int aBorderWidth = aBorderSize;

	CFontData* fontData = nullptr;


	std::stringstream key;
	key << aFontPath << "-" << aFontWidth;


	if (myFontData.find(key.str()) == myFontData.end())
	{
		fontData = new CFontData;
		myFontData[key.str()] = fontData;

	}
	else
	{
		return myFontData[key.str()];
	}

	fontData->myAtlas = new int[atlasSize * atlasSize];
	ZeroMemory(fontData->myAtlas, (atlasSize * atlasSize) * sizeof(int));
	fontData->myFontHeightWidth = aFontWidth;
	fontData->myName = key.str();

	FT_Face face;
	FT_Error error = FT_New_Face(myLibrary, aFontPath.c_str(), 0, &face);
	if (error != 0)
	{
		return nullptr;
	}

	FT_F26Dot6 ftSize = (FT_F26Dot6)(fontData->myFontHeightWidth * (1 << 6));
	error = FT_Set_Char_Size(face, ftSize, 0, 100, 0); // 96 = 100% scaling in Windows. 
	if (error != 0)
	{
		DL_ERROR(eDebugLogType::Resource,"%s", "FT_Set_Char_Size error");
		return nullptr;
	}

	error = FT_Load_Char(face, 'x', FT_LOAD_DEFAULT);
	if (error != 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Failed to load glyph!");
		return nullptr;
	}


	FT_GlyphSlot space = face->glyph;

	fontData->myWordSpacing = static_cast<short>(space->metrics.width / 256);

	int currentMax = 256;
	int currentI = 32;

	for (int i = currentI; i < currentMax; i++)
	{
		error = FT_Load_Char(face, i, FT_LOAD_RENDER);
		if (error != 0)
		{
			DL_ERROR(eDebugLogType::Resource, "%s%s", "Failed to load glyph! ", (char)i);
		}
		FT_GlyphSlot slot = face->glyph;

		if (atlasX + slot->bitmap.width + (aBorderWidth * 2) > atlasWidth)
		{
			atlasX = X_START;
			atlasY = currentMaxY;
		}
		if (aBorderWidth > 0)
		{
			LoadOutline(i, atlasX, atlasY, atlasWidth, fontData, face, aBorderWidth);
		}
		LoadGlyph(i, atlasX, atlasY, currentMaxY, atlasWidth, atlasHeight, fontData, face, aBorderWidth);
	}

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = fontData->myAtlas;
	data.SysMemPitch = atlasSize * 4;

	D3D11_TEXTURE2D_DESC info;
	info.Width = atlasSize;
	info.Height = atlasSize;
	info.MipLevels = 1;
	info.ArraySize = 1;
	info.SampleDesc.Count = 1;
	info.SampleDesc.Quality = 0;
	info.MiscFlags = 0;
	info.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	info.Usage = D3D11_USAGE_DYNAMIC;
	info.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	info.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ID3D11Texture2D* texture;
	CEngine::GetInstance()->GetDirect3D().GetDevice()->CreateTexture2D(&info, &data, &texture);
	if (error != 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Failed to load texture for text!");
		return nullptr;
	}
	CEngine::GetInstance()->GetDirect3D().GetDevice()->CreateShaderResourceView(texture, nullptr, &fontData->myAtlasView);
	texture->Release();

	fontData->myAtlasHeight = atlasSize;
	fontData->myAtlasWidth = atlasSize;
	fontData->myLineSpacing = (face->ascender - face->descender) / 64.f;
	FT_Done_Face(face);

	fontData->myBatch = new CSpriteBatch(false);
	fontData->myBatch->Init(nullptr);
	fontData->myBatch->myQuadBatch->myIsScreenSpace = true;

	CTexture* tex = new CTexture();
	fontData->myBatch->myQuadBatch->myTexture = tex;
	fontData->myBatch->myQuadBatch->myTexture->myResource = fontData->myAtlasView;
	fontData->myBatch->myQuadBatch->myTexture->myPath = key.str();

	return fontData;

}

void CTextService::Update()
{
	myCurrentSprite = 0;
}


float CTextService::GetSentenceWidth(CText* aText)
{
	if (!aText)
	{
		return 0.0f;
	}
	if (aText->myText.size() <= 0)
	{
		return 0.0f;
	}

	CFontData* fontData = aText->myFontDataPointer;
	if (!fontData)
	{
		return 0.0f;
	}

	float drawX = 0.f;

	int count = static_cast<int>(aText->myText.length());

	// Compute middle
	for (int i = 0; i < count; i++)
	{
		SCharData& charData = fontData->myCharData[(int)aText->myText[i]];

		if (aText->myText[i] == ' ')
		{
			drawX += fontData->myWordSpacing;
		}

		drawX += charData.myBearingX;
	}

	return drawX * aText->myScale;
}

std::string CTextService::GetRenderName(const std::string& aFontPathAndName, EFontSize aSize)
{
	std::stringstream key;
	key << aFontPathAndName << "-" << aSize;

	return key.str();
}

void CTextService::ClearMemory(const std::string& aRenderName)
{
	CFontData* fontData = myFontData[aRenderName];
	if (!fontData)
	{
		return;
	}

	delete myFontData[aRenderName];
	myFontData.erase(aRenderName);
}

bool CTextService::UpdateTextAndDraw(const std::string& aText, const Vector2f& aPosition, Vector4f aColor, float aSize, CFontData* aDataPtr)
{
	CFontData* fontData = aDataPtr;
	if (!fontData || !fontData->myBatch)
	{
		return false;
	}

	fontData->myBufferedObjects.ResetCount();
	BuildText(aText, aColor, aSize, aDataPtr, fontData->myBufferedObjects);
	
	float screenSizeX = static_cast<float>(CEngine::GetInstance()->GetWindowSize().x);
	float screenSizeY = static_cast<float>(CEngine::GetInstance()->GetWindowSize().y);
	const float screewnWidthOffsetSpace = 0.05f;
	float positionOffsetX = (screenSizeX - (screenSizeX * screewnWidthOffsetSpace));
	if (fontData->myRenderCountChanged)
	{
		fontData->myRenderCountChanged = false;
		fontData->myBatch->ClearAll();
	}

	for (int i = 0; i < fontData->myBufferedObjects.NextCount()+1; i++)
	{
		if (i > FONT_BUFF_SIZE)
		{
			break;
		}
		STextToRender* text = fontData->myBufferedObjects.GetAt(i);
		if (myCurrentSprite < MAX_SPRITES)
		{
			CSprite* sprite = &mySprites[myCurrentSprite];

			
			text->myPosition.x /= positionOffsetX;
			text->myPosition.y /= screenSizeY;

			text->mySize.x /= screenSizeY;
			text->mySize.y /= screenSizeY;

			sprite->SetPosition(text->myPosition + aPosition);
			sprite->SetSizeRelativeToScreen(text->mySize * aSize);
			sprite->SetColor(text->myColor);

			sprite->SetTextureRect(text->myUV.x, text->myUV.y, text->uvScale.x, text->uvScale.y);
			fontData->myBatch->AddObject(sprite);
			myCurrentSprite++; 
		}
	}

	fontData->myBatch->Render();

	return true;
}

void CTextService::BuildText(const std::string& aText, Vector4f aColor, float aSize, CFontData* aDataPtr, Corgi::SSimplePointerArray<STextToRender*>& aTextBuffer)
{
	CFontData* fontData = aDataPtr;
	if (!fontData)
	{
		return;
	}

	int count = static_cast<int>(aText.length());
	if (aDataPtr->myLastRenderCount != count)
	{
		aDataPtr->myLastRenderCount = count;
		aDataPtr->myRenderCountChanged = true;
	}
	float drawX = 0.f;
	float drawY = -5.f;
	float maxDrawY = 0.f;
	 
	for (int i = 0, row = 0; i < count; i++)
	{
		SCharData& charData = fontData->myCharData[(int)aText[i]];

		if (maxDrawY < charData.myHeight)
		{
			maxDrawY = static_cast<float>(charData.myHeight);
		}


		if (aText[i] == '\n')
		{
			drawX = 0;
			drawY -= (maxDrawY + 6);
			row++;
			continue;
		}


		if (aText[i] == ' ')
		{
			drawX += fontData->myWordSpacing;
		}

		float top = drawY + charData.myBearingY;
		float bottom = top + charData.myHeight;

		STextToRender* renderText = aTextBuffer.GetNext();

		renderText->myPosition = {drawX * aSize, -bottom * aSize};
		renderText->mySize = { static_cast<float>(charData.myWidth), static_cast<float>(charData.myHeight) };


		renderText->myUV = charData.myTopLeftUV;
		renderText->uvScale = charData.myBottomRightUV;
		renderText->myColor = aColor;


		drawX += charData.myAdvanceX;
	}
}

bool CTextService::AddTextToRender(const std::string& aText, const Vector2f& aPosition, Vector4f aColor, float aSize, CFontData* aDataPtr)
{
	// Call draw directly, students are suppose to create buffers themselves
	return UpdateTextAndDraw(aText, aPosition, aColor, aSize, aDataPtr);
}

void CTextService::CountOffsets(const int& x, const int& y, const int& width, const int& height, SCountData& countData)
{
	if (x < 1)
	{
		countData.xNCount++;
	}

	if (x > width - 1)
	{
		countData.xPCount++;
	}

	if (y < 1)
	{
		countData.yNCount++;
	}

	if (y > height - 1)
	{
		countData.yPCount++;
	}
}

void CTextService::CountDeltas(const int& width, const int& height, int& deltaX, int& deltaY, SCountData& countData)
{
	if (countData.xNCount == height)
	{
		countData.xNCount = 0;
		deltaX--;
	}

	if (countData.xPCount == height)
	{
		countData.xPCount = 0;
		deltaX++;
	}

	if (countData.yNCount == width)
	{
		countData.yNCount = 0;
		deltaY--;
	}

	if (countData.yPCount == width)
	{
		countData.yPCount = 0;
		deltaY++;
	}
}

void CTextService::CalculateOutlineOffsets(const int index, FT_FaceRec_* aFace, int aBorderOffset)
{
	myOffset.xDelta = 0;
	myOffset.yDelta = 0;

	FT_Error err;
	FT_Stroker stroker;
	FT_Glyph glyph;

	err = FT_Load_Char(aFace, index, FT_LOAD_NO_BITMAP);
	if (err != 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Failed to load glyph!");
	}
	err = FT_Get_Glyph(aFace->glyph, &glyph);

	glyph->format = FT_GLYPH_FORMAT_OUTLINE;

	err = FT_Stroker_New(myLibrary, &stroker);
	if (err != 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Failed to get glyph!");
	}
	

	FT_Stroker_Set(stroker, aBorderOffset * 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
	err = FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
	if (err != 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Failed to stroke glyph border");
	}


	err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
	if (err != 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Failed to add glyph to bitmap");
	}
	
	FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);

	unsigned int width = bitmapGlyph->bitmap.width;
	unsigned int height = bitmapGlyph->bitmap.rows;

	int xDelta = 0;
	int yDelta = 0;

	SCountData countData;
	for (unsigned int x = 0; x < width; x++)
	{
		for (unsigned int y = 0; y < height; y++)
		{
			unsigned char& data = bitmapGlyph->bitmap.buffer[y * width + x];

			if (data == 0)
			{
				CountOffsets(x, y, width, height, countData);
			}
			CountDeltas(width, height, xDelta, yDelta, countData);
		}
	}

	myOffset.xDelta = xDelta;
	myOffset.yDelta = yDelta;

	FT_Stroker_Done(stroker);
}

void CTextService::CalculateGlyphOffsets(const int /*index*/, FT_GlyphSlotRec_* glyph)
{
	int xDelta = 0;
	int yDelta = 0;

	unsigned int width = glyph->bitmap.width;
	unsigned int height = glyph->bitmap.rows;

	SCountData countData;
	for (unsigned int x = 0; x < width; x++)
	{
		for (unsigned int y = 0; y < height; y++)
		{
			unsigned char& data = glyph->bitmap.buffer[y * width + x];

			if (data == 0)
			{
				CountOffsets(x, y, width, height, countData);
			}
			CountDeltas(width, height, xDelta, yDelta, countData);
		}
	}

	myOffset.xDelta = xDelta;
	myOffset.yDelta = yDelta;
}

int Color32Reverse2(int x)
{
	return ((x & 0xFF000000) >> 8) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) | ((x & 0x000000FF) << 24);
}

int Color32Reverse(int x)
{
	int returner = x;
	returner |= ((x & 0x000000FF) << 24);
	returner |= ((x & 0x000000FF) << 16);
	returner |= ((x & 0x000000FF) << 8);

	return returner;
}


void CTextService::LoadGlyph(int index, int& atlasX, int& atlasY, int& maxY, float atlasWidth, float atlasHeight, CFontData* aFontData, FT_FaceRec_* aFace, int aBorderOffset)
{
	FT_Error error = FT_Load_Char(aFace, index, FT_LOAD_RENDER);
	if (error != 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Failed to load char while glyph loading");
	}

	FT_GlyphSlot slot = aFace->glyph;
	FT_Bitmap bitmap = slot->bitmap;

	int height = bitmap.rows;
	int width = bitmap.width;

	const float bearingValue = 64.0f;
	SCharData glyphData;
	glyphData.myChar = static_cast<char>(index); 
	glyphData.myHeight = static_cast<short>(height + (aBorderOffset * 2));
	glyphData.myWidth = static_cast<short>(width + (aBorderOffset * 2));


	glyphData.myTopLeftUV = { (float(atlasX) / atlasWidth), (float(atlasY) / atlasHeight) };
	glyphData.myBottomRightUV = { (float(atlasX + glyphData.myWidth) / atlasWidth), (float(atlasY + glyphData.myHeight) / atlasHeight) };
	 
	glyphData.myAdvanceX = static_cast<short>(slot->metrics.horiAdvance / bearingValue);
	if (glyphData.myChar == '1')
	{
		glyphData.myAdvanceX = (short)((float)glyphData.myAdvanceX * 1.4f);
	}
	glyphData.myBearingX = static_cast<short>(((slot->metrics.horiBearingX + slot->metrics.width) / bearingValue) + (aBorderOffset * 2));

	glyphData.myBearingY = static_cast<short>(((slot->metrics.horiBearingY - slot->metrics.height) / bearingValue));

	if (glyphData.myTopLeftUV.x > 1 || glyphData.myTopLeftUV.y > 1 || glyphData.myBottomRightUV.x > 1 || glyphData.myBottomRightUV.y > 1)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Tried to set a glyph UV to above 1");
	} 


	CalculateGlyphOffsets(index, slot);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (x < 0 || y < 0)
			{
				continue;
			}
			int& saved = aFontData->myAtlas[((atlasY)+aBorderOffset + myOffset.yDelta + y) *
				int(atlasWidth) + ((atlasX + aBorderOffset + myOffset.xDelta) + x)];
			saved |= bitmap.buffer[y * bitmap.width + x];

			saved = Color32Reverse(saved);

			if (y + (atlasY + Y_OFFSET) > maxY)
			{
				maxY = y + (atlasY + Y_OFFSET);
			}
		}
	}

	atlasX = atlasX + width + X_OFFSET;
	aFontData->myCharData[index] = glyphData;
}

void CTextService::LoadOutline(const int index, const int atlasX, const int atlasY , const float atlasWidth, CFontData* aFontData, FT_FaceRec_* aFace, int aBorderOffset)
{

	FT_Error err;
	FT_Stroker stroker;
	FT_Glyph glyph;

	err = FT_Load_Char(aFace, index, FT_LOAD_NO_BITMAP);
	if (err != 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Failed to load glyph!");
	}

	err = FT_Get_Glyph(aFace->glyph, &glyph);
	if (err != 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Failed to get glyph!");
	}

	glyph->format = FT_GLYPH_FORMAT_OUTLINE;

	err = FT_Stroker_New(myLibrary, &stroker);
	if (err != 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Failed to get stroker!");
	}


	FT_Stroker_Set(stroker, aBorderOffset * 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
	err = FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
	if (err != 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Failed to stroke!");
	}


	err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, true);
	if (err != 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Failed to add glyph to bitmap");
	}
	
	FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);

	unsigned int width = bitmapGlyph->bitmap.width;
	unsigned int height = bitmapGlyph->bitmap.rows;

	CalculateOutlineOffsets(index, aFace, aBorderOffset);
	for (unsigned  int x = 0; x < width; x++)
	{
		for (unsigned int y = 0; y < height; y++)
		{

			int& data = aFontData->myAtlas[((atlasY + myOffset.yDelta) + y) * int(atlasWidth) + ((atlasX + myOffset.xDelta) + x)];
			data = 0;
			data |= bitmapGlyph->bitmap.buffer[y * width + x];
			data = Color32Reverse2(data);

		}
	}

	FT_Stroker_Done(stroker);
}