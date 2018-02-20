#pragma once
struct SNavmeshData
{
	unsigned int myVertexBufferSize;
	int myVertexCount;
	char* myVertexes;

	unsigned int myIndexBufferSize;
	unsigned int myIndexCount;
	unsigned int* myIndexes;
	int myShaderType;
};