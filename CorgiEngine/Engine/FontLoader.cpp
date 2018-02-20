#include "stdafx.h"
#include "FontLoader.h"
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


#define X_OFFSET 8
#define X_START 2
#define Y_OFFSET 8


CFontLoader::CFontLoader(CEngine& aEngine) :
	myEngine(aEngine)
{
	FT_Init_FreeType(&myLibrary);
}


CFontLoader::~CFontLoader()
{
}

bool CFontLoader::InitAndLoad(CFontData& aFontDataOut, std::string aFontPathAndName, unsigned char aBorderSize, int aSize)
{
	if (aSize == -1)
	{
		return false;
	}

	short aFontWidth = (short)aSize;
	int atlasSize = 1024;
	float atlasWidth = static_cast<float>(atlasSize);
	float atlasHeight = static_cast<float>(atlasSize);

	int atlasX = X_START;
	int atlasY = 2;
	int currentMaxY = 0;

	int aBorderWidth = aBorderSize;



	std::stringstream key;
	key << aFontPathAndName << "-" << aFontWidth;

	aFontDataOut.myAtlas = new int[atlasSize * atlasSize];
	ZeroMemory(aFontDataOut.myAtlas, (atlasSize * atlasSize) * sizeof(int));
	aFontDataOut.myFontHeightWidth = aFontWidth;
	aFontDataOut.myName = key.str();

	FT_Face face;
	FT_Error error = FT_New_Face(myLibrary, aFontPathAndName.c_str(), 0, &face);
	if (error != 0)
	{
		return false;
	}

	FT_F26Dot6 ftSize = (FT_F26Dot6)(aFontDataOut.myFontHeightWidth * (1 << 6));
	error = FT_Set_Char_Size(face, ftSize, 0, 100, 0); // 96 = 100% scaling in Windows. 
	if (error != 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "FT_Set_Char_Size error");
		return false;
	}

	error = FT_Load_Char(face, 'x', FT_LOAD_DEFAULT);
	if (error != 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Failed to load glyph!");
		return false;
	}


	FT_GlyphSlot space = face->glyph;

	aFontDataOut.myWordSpacing = static_cast<short>(space->metrics.width / 256);

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
			LoadOutline(i, atlasX, atlasY, atlasWidth, &aFontDataOut, face, aBorderWidth);
		}
		LoadGlyph(i, atlasX, atlasY, currentMaxY, atlasWidth, atlasHeight, &aFontDataOut, face, aBorderWidth);
	}

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = aFontDataOut.myAtlas;
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
	myEngine.GetDXFrameWork()->GetDevice()->CreateTexture2D(&info, &data, &texture);
	if (error != 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "Failed to load texture for text!");
		return false;
	}
	myEngine.GetDXFrameWork()->GetDevice()->CreateShaderResourceView(texture, nullptr, &aFontDataOut.myAtlasView);
	texture->Release();

	aFontDataOut.myAtlasHeight = atlasSize;
	aFontDataOut.myAtlasWidth = atlasSize;
	aFontDataOut.myLineSpacing = (face->ascender - face->descender) / 64.f;
	FT_Done_Face(face);

	//aFontDataOut.myBatch = new CSpriteBatch(false);
	//aFontDataOut.myBatch->Init(nullptr);
	//aFontDataOut.myBatch->myQuadBatch->myIsScreenSpace = true;
	//
	//CTexture* tex = new CTexture();
	//aFontDataOut.myBatch->myQuadBatch->myTexture = tex;
	//aFontDataOut.myBatch->myQuadBatch->myTexture->myResource = aFontDataOut.myAtlasView;
	//aFontDataOut.myBatch->myQuadBatch->myTexture->myPath = key.str();
	DL_SUCCESS(eDebugLogType::Resource, "%s%s%s", "[Fontloader] - Font: ", aFontPathAndName.c_str(), " loaded");
	return true;
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

void CFontLoader::LoadGlyph(int index, int & atlasX, int & atlasY, int & maxY, float atlasWidth, float atlasHeight, CFontData * aFontData, FT_FaceRec_ * aFace, int aBorderWidth)
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
	glyphData.myHeight = static_cast<short>(height + (aBorderWidth * 2));
	glyphData.myWidth = static_cast<short>(width + (aBorderWidth * 2));


	glyphData.myTopLeftUV = { (float(atlasX) / atlasWidth), (float(atlasY) / atlasHeight) };
	glyphData.myBottomRightUV = { (float(atlasX + glyphData.myWidth) / atlasWidth), (float(atlasY + glyphData.myHeight) / atlasHeight) };

	glyphData.myAdvanceX = static_cast<short>(slot->metrics.horiAdvance / bearingValue);
	if (glyphData.myChar == '1')
	{
		glyphData.myAdvanceX = (short)((float)glyphData.myAdvanceX * 1.4f);
	}
	glyphData.myBearingX = static_cast<short>(((slot->metrics.horiBearingX + slot->metrics.width) / bearingValue) + (aBorderWidth * 2));

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
			int& saved = aFontData->myAtlas[((atlasY)+aBorderWidth + myOffset.yDelta + y) *
				int(atlasWidth) + ((atlasX + aBorderWidth + myOffset.xDelta) + x)];
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

void CFontLoader::LoadOutline(const int index, const int atlasX, const int atlasY, const float atlasWidth, CFontData * aFontData, FT_FaceRec_ * aFace, int aBorderWidth)
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


	FT_Stroker_Set(stroker, aBorderWidth * 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
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

	CalculateOutlineOffsets(index, aFace, aBorderWidth);
	for (unsigned int x = 0; x < width; x++)
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

void CFontLoader::CalculateOutlineOffsets(const int index, FT_FaceRec_ * aFace, int aBorderWidth)
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


	FT_Stroker_Set(stroker, aBorderWidth * 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
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

void CFontLoader::CalculateGlyphOffsets(const int index, FT_GlyphSlotRec_ * glyph)
{
	index;
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

void CFontLoader::CountOffsets(const int & x, const int & y, const int & width, const int & height, SCountData & countData)
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

void CFontLoader::CountDeltas(const int & width, const int & height, int & deltaX, int & deltaY, SCountData & countData)
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

CFontData::~CFontData()
{
}
