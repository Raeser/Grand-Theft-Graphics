#pragma once

#include "D3D11DataWrappers.h"

class CSprite
{
	friend class CSpriteLoader;
public:
	CSprite();
	~CSprite();

	struct SSpriteData
	{
		SVertexDataWrapper myVertexData;
		SShaderDataWrapper myShaderData;
		SLayoutDataWrapper myLayoutData;
		STextureDataWrapper myTextureData;
	};

	bool Init(SSpriteData aModelData);

	bool SetSpriteData(SSpriteData aModelData);

	SVertexDataWrapper& GetVertexData() { return mySpriteData.myVertexData; }
	SShaderDataWrapper& GetShaderData() { return mySpriteData.myShaderData; }
	SLayoutDataWrapper& GetLayoutData() { return mySpriteData.myLayoutData; }
	STextureDataWrapper& GetTextureData() { return mySpriteData.myTextureData; }
	const Vector2f& GetOriginalSize();
private:
	void SetOriginalSize(Vector2f aSize);
	SSpriteData mySpriteData;
	Vector2f myOriginalSize;
};

