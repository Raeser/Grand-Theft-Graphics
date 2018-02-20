#pragma once

// Do not change here!!

struct SModelData 
{
	unsigned int myVertexBufferSize;
	int myVertexCount;
	char* myVertexes;

	unsigned int myIndexBufferSize;
	unsigned int myIndexCount;
	unsigned int* myIndexes;
	int myShaderType;
	void* myScene;
	std::vector<std::string> myTextures;
};

enum TextureEnums
{
	eAlbedo,
	eRoughness,
	eAmbient,
	eEmissive,
	eHeight,
	eNormals,
	eShininess,
	eOpacity,
	eDisplacement,
	eLightMap,
	eMetalness
};