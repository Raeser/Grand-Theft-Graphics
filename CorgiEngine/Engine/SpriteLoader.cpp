#include "stdafx.h"
#include "SpriteLoader.h"
#include "Sprite.h"
#include "SpriteInstance.h"
#include "DDSTextureLoader.h"

CSpriteLoader::CSpriteLoader()
{
	mySprites.Init(3000);
	myLoadedSpritePaths.Init(3000);
	myInstanceContainers.Init(eScene_count);
	for (int i = 0; i < eScene_count; ++i)
	{
		Corgi::GrowingArray<CSpriteInstance> ContainerToAdd;
		ContainerToAdd.Init(1024);
		myInstanceContainers.Add(ContainerToAdd);
	}

}


CSpriteLoader::~CSpriteLoader()
{

}

void CSpriteLoader::Init(Vector2f aScreenSize, ID3D11Device* aDevice, eScene aSceneToStartWith)
{
	myCurrentScene = aSceneToStartWith;
	myScreenSize = aScreenSize;
	myDevice = aDevice;
	CreateSprite("../Assets/Sprites/errorSprite.dds");
	//long long cursorID = CreateSprite("../Assets/Sprites/Menus/mouseCursor.dds");

	//myCursor.SetSprite(cursorID);
}

CSpriteInstance * CSpriteLoader::CreateSpriteInstance(long long aID)
{
	CSpriteInstance returnMe;
	returnMe.SetSprite(aID);
	myInstanceContainers[myCurrentScene].Add(returnMe);
	return &myInstanceContainers[myCurrentScene].GetLast();
}

long long CSpriteLoader::CreateSprite(const char * aPath)
{
	std::string path = aPath;
	Corgi::ConvertStringPathToSafeUse(path);
	long long IdToReturn = myLoadedSpritePaths.Find(path);
	if (IdToReturn != -1)
	{
		return IdToReturn;
	}
	DL_WRITELOG(eConsoleColor::LIGHTBLUE,eDebugLogType::Resource, "%s%s", "[CSpriteLoader] - Creating sprite : ", path.c_str());
	HRESULT result;


	ID3D10Blob* VSBlob;
	ID3D10Blob* PSBlob;
	myShaderCompiler.CompileShader("../Assets/Shaders/VS_PBLSprite.hlsl", "main", "vs_5_0", VSBlob);
	myShaderCompiler.CompileShader("../Assets/Shaders/PS_PBLSprite.hlsl", "main", "ps_5_0", PSBlob);

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	result = myDevice->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &vertexShader);
	result = myDevice->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &pixelShader);

	//Texture
	ID3D11ShaderResourceView* texture;
	ID3D11Resource* resource;
	std::string TexturePath = aPath;
	std::wstring filename;
	filename.assign(TexturePath.begin(), TexturePath.end());
	result = DirectX::CreateDDSTextureFromFile(myDevice, filename.c_str(), &resource, &texture);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s%s", "[CSpriteLoader] - Texture failed to load, Rendering Error sprite instead! Path Was: ", aPath);
		return 0;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "[CSpriteLoader] - Texture load succeded");
	}
	Vector2f sizeOfSprite = GetSpriteSize(resource);
	//end load texture

	//Create triangle vertices
	Vertex vertices[4] =
	{
		{ -sizeOfSprite.x , sizeOfSprite.y, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f },
		{ sizeOfSprite.x, sizeOfSprite.y, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 0.f },
		{ -sizeOfSprite.x, -sizeOfSprite.y, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f },
		{ sizeOfSprite.x, -sizeOfSprite.y, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 1.f }
	};
	unsigned int indices[6] = { 
		0,1,2,
		1,3,2
	};

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.ByteWidth = sizeof(vertices);
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA bufferSubResourceData = { 0 };
	bufferSubResourceData.pSysMem = vertices;

	ID3D11Buffer* vertexBuffer;
	result = myDevice->CreateBuffer(&bufferDescription, &bufferSubResourceData, &vertexBuffer);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[CSpriteLoader] - VertexBuffer failed to load");
		return 0;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "[CSpriteLoader] - VertexBuffer load succeded");
	}
	D3D11_BUFFER_DESC indexBufferDescription = {};
	indexBufferDescription.ByteWidth = sizeof(indices);
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferSubResourceData = { 0 };
	indexBufferSubResourceData.pSysMem = indices;

	ID3D11Buffer* indexBuffer;
	result = myDevice->CreateBuffer(&indexBufferDescription, &indexBufferSubResourceData, &indexBuffer);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[CSpriteLoader] - IndexBuffer failed to load");
		return 0;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "[CSpriteLoader] - IndexBuffer load succeded");
	}
	//Create Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0 , DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ID3D11InputLayout* inputLayout;
	result = myDevice->CreateInputLayout(layout, 3, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &inputLayout);
	//End creation of Layout
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[CSpriteLoader] - InputLayout failed to load");
		return 0;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "[CSpriteLoader] - InputLayout load succeded");
	}
	//Create and return(?) model

	CSprite::SSpriteData spriteData;
	spriteData.myVertexData.myNumberOfVertices = sizeof(vertices) / sizeof(Vertex);
	spriteData.myVertexData.myNumberOfIndices = sizeof(indices) / sizeof(UINT);
	spriteData.myVertexData.myStride = sizeof(Vertex);
	spriteData.myVertexData.myOffset = 0;
	spriteData.myVertexData.myVertexBuffer = vertexBuffer;
	spriteData.myVertexData.myIndexBuffer = indexBuffer;
	spriteData.myShaderData.myVertexShader = vertexShader;
	spriteData.myShaderData.myPixelShader = pixelShader;
	spriteData.myLayoutData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	spriteData.myLayoutData.myInputLayout = inputLayout;
	spriteData.myTextureData.myAlbedo = texture;

	CSprite returnMe;
	returnMe.Init(spriteData);
	returnMe.SetOriginalSize(sizeOfSprite);
	mySprites.Add(returnMe);
	myLoadedSpritePaths.Add(path);
	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Resource, "%s", "[CSpriteLoader] - Done");
	Corgi::CFileWatcher::GetInstance().WatchFileChange(Corgi::ConvertCharPtrToWideString(myLoadedSpritePaths[mySprites.Size() - 1].c_str()), std::bind(&CSpriteLoader::ReLoadSprite, this, mySprites.Size() - 1));
	return mySprites.Size() - 1;
}

Vector2f CSpriteLoader::GetSpriteSize(ID3D11Resource * aResource)
{
	D3D11_RESOURCE_DIMENSION resType = D3D11_RESOURCE_DIMENSION_UNKNOWN;
	aResource->GetType(&resType);
	Vector2f sizeOfSprite;
	switch (resType)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		ID3D11Texture1D* texture1D;
		aResource->QueryInterface<ID3D11Texture1D>(&texture1D);
		D3D11_TEXTURE1D_DESC desc1D;
		texture1D->GetDesc(&desc1D);
		sizeOfSprite.x = desc1D.Width / myScreenSize.x;
		sizeOfSprite.y = 0;
		DL_WARNING(eDebugLogType::Resource, "%s", "Sprite Was 1D, that's weird?");
		break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		ID3D11Texture2D* texture2D;
		aResource->QueryInterface<ID3D11Texture2D>(&texture2D);
		D3D11_TEXTURE2D_DESC desc2D;
		texture2D->GetDesc(&desc2D);
		sizeOfSprite.x = (desc2D.Width * 1.f);
		sizeOfSprite.y = (desc2D.Height * 1.f);
		//DL_SUCCESS(eDebugLogType::Resource, "%s", "Sprite Was 2D, Good!");
		break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		ID3D11Texture3D* texture3D;
		aResource->QueryInterface<ID3D11Texture3D>(&texture3D);
		D3D11_TEXTURE3D_DESC desc3D;
		texture3D->GetDesc(&desc3D);
		sizeOfSprite.x = desc3D.Width / myScreenSize.x;
		sizeOfSprite.y = desc3D.Height / myScreenSize.y;
		DL_WARNING(eDebugLogType::Resource, "%s", "Sprite Was 3D, that's weird?");
		break;
	default:
		break;
	}
	return sizeOfSprite;
}

bool CSpriteLoader::ReLoadSprite(long long aID)
{
	DL_WRITELOG(eConsoleColor::LIGHTBLUE, eDebugLogType::Resource, "%s%s", "[CSpriteLoader] - Recreating sprite : ", myLoadedSpritePaths[aID].c_str());
	if (aID > myLoadedSpritePaths.Size() - 1 || aID < 0)
	{
		DL_ERROR(eDebugLogType::Resource, "%s%s%s", "[CSpriteLoader] - Failed to reload sprite: ", myLoadedSpritePaths[aID].c_str(), "ID number is out of bounds");
		return false;
	}
	HRESULT result;


	ID3D10Blob* VSBlob;
	ID3D10Blob* PSBlob;
	myShaderCompiler.CompileShader("../Assets/Shaders/VS_PBLSprite.hlsl", "main", "vs_5_0", VSBlob);
	myShaderCompiler.CompileShader("../Assets/Shaders/PS_PBLSprite.hlsl", "main", "ps_5_0", PSBlob);

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	result = myDevice->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &vertexShader);
	result = myDevice->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &pixelShader);

	//Texture
	ID3D11ShaderResourceView* texture;
	ID3D11Resource* resource;
	std::string TexturePath = myLoadedSpritePaths[aID];
	std::wstring filename;
	filename.assign(TexturePath.begin(), TexturePath.end());
	result = DirectX::CreateDDSTextureFromFile(myDevice, filename.c_str(), &resource, &texture);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s%s", "[CSpriteLoader] - Texture failed to load, Rendering Error sprite instead! Path Was: ", myLoadedSpritePaths[aID].c_str());
		return 0;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "[CSpriteLoader] - Texture load succeded");
	}
	Vector2f sizeOfSprite = GetSpriteSize(resource);
	//end load texture

	//Create triangle vertices
	Vertex vertices[4] =
	{
		{ -sizeOfSprite.x , sizeOfSprite.y, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f },
		{ sizeOfSprite.x, sizeOfSprite.y, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 0.f },
		{ -sizeOfSprite.x, -sizeOfSprite.y, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f },
		{ sizeOfSprite.x, -sizeOfSprite.y, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 1.f }
	};
	unsigned int indices[6] = {
		0,1,2,
		1,3,2
	};

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.ByteWidth = sizeof(vertices);
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA bufferSubResourceData = { 0 };
	bufferSubResourceData.pSysMem = vertices;

	ID3D11Buffer* vertexBuffer;
	result = myDevice->CreateBuffer(&bufferDescription, &bufferSubResourceData, &vertexBuffer);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[CSpriteLoader] - VertexBuffer failed to load");
		return 0;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "[CSpriteLoader] - VertexBuffer load succeded");
	}
	D3D11_BUFFER_DESC indexBufferDescription = {};
	indexBufferDescription.ByteWidth = sizeof(indices);
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferSubResourceData = { 0 };
	indexBufferSubResourceData.pSysMem = indices;

	ID3D11Buffer* indexBuffer;
	result = myDevice->CreateBuffer(&indexBufferDescription, &indexBufferSubResourceData, &indexBuffer);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[CSpriteLoader] - IndexBuffer failed to load");
		return 0;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "[CSpriteLoader] - IndexBuffer load succeded");
	}
	//Create Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0 , DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ID3D11InputLayout* inputLayout;
	result = myDevice->CreateInputLayout(layout, 3, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &inputLayout);
	//End creation of Layout
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[CSpriteLoader] - InputLayout failed to load");
		return 0;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "[CSpriteLoader] - InputLayout load succeded");
	}
	//Create and return(?) model

	CSprite::SSpriteData spriteData;
	spriteData.myVertexData.myNumberOfVertices = sizeof(vertices) / sizeof(Vertex);
	spriteData.myVertexData.myNumberOfIndices = sizeof(indices) / sizeof(UINT);
	spriteData.myVertexData.myStride = sizeof(Vertex);
	spriteData.myVertexData.myOffset = 0;
	spriteData.myVertexData.myVertexBuffer = vertexBuffer;
	spriteData.myVertexData.myIndexBuffer = indexBuffer;
	spriteData.myShaderData.myVertexShader = vertexShader;
	spriteData.myShaderData.myPixelShader = pixelShader;
	spriteData.myLayoutData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	spriteData.myLayoutData.myInputLayout = inputLayout;
	spriteData.myTextureData.myAlbedo = texture;

	mySprites[aID].SetSpriteData(spriteData);
	//mySprites[aID].SetOriginalSize(sizeOfSprite);
	return true;
}

CSprite & CSpriteLoader::GetSprite(long long aID)
{
	if (aID > mySprites.Size() - 1 || aID == -1)
	{
		//DL_WARNING(eDebugLogType::Resource, "%s", "tried to use sprite with an invalid id, defaulting to error sprite!");
		return mySprites[0];
	}
	return mySprites[aID];
}


void CSpriteLoader::SetActiveScene(eScene aActiveScene)
{
	myCurrentScene = aActiveScene;
}

const Corgi::GrowingArray<CSpriteInstance>& CSpriteLoader::GetCurrentInstances() const
{
	return myInstanceContainers[myCurrentScene];
}

void CSpriteLoader::UpdateActiveSpriteInstances()
{
	for (int i = 0; i < myInstanceContainers[myCurrentScene].Size(); ++i)
	{
		if (myInstanceContainers[myCurrentScene][i].GetShouldRemove())
		{
			myInstanceContainers[myCurrentScene].RemoveCyclicAtIndex(i);
		}
	}
}

