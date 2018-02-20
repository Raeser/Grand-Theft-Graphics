#include "stdafx.h"
#include "ModelLoader.h"
#include "DDSTextureLoader.h"
#include "FBXLoaderCustom.h"
#include "ConstModelStructs.h"
#include "Model.h"
#include "AnimationLoader.h"

CModelLoader::CModelLoader(CShaderManager & aShaderManager, CAnimationLoader& aAnimLoader) : myShaderManager(aShaderManager), myAnimationLoader(aAnimLoader)
{
	myLoadedModelPaths.Init(64);
	myFailedModelPaths.Init(64);
	myModels.Init(64);
	myLoader = new CFBXLoaderCustom();
	myIsInited = false;
}


CModelLoader::~CModelLoader()
{
	
}

bool CModelLoader::Init(ID3D11Device * aDevice)
{
	if (myIsInited == false)
	{
		myDevice = aDevice;
		myFullBlackErrorTexture = CreateDefaultTexture(false, eBlack);
		myChequeredErrorTexture = CreateDefaultTexture(true, eNormalColor);
		myFullWhiteErrorTexture = CreateDefaultTexture(false, eWhite);
		//CreateCube();
		myIsInited = true;
	return true;
	}
	return false;
}

void CModelLoader::Release()
{
	myChequeredErrorTexture->Release();
	myChequeredErrorTexture = nullptr;

	myFullBlackErrorTexture->Release();
	myFullBlackErrorTexture = nullptr;

	myFullWhiteErrorTexture->Release();
	myFullWhiteErrorTexture = nullptr;
}

CModelInstance * CModelLoader::CreateInstance()
{
	CModelInstance* returnMe = new CModelInstance();
	returnMe->SetModel(-1);

	myScene->AddModelInstance(returnMe);


	return returnMe;
}

CModelInstance * CModelLoader::CreateInstance2(long long aModelID)
{
	CModelInstance* returnMe = new CModelInstance();
	returnMe->SetModel(aModelID);
	return returnMe;
}

CModelInstance * CModelLoader::CreateInstance(long long aModelID)
{
	CModelInstance* returnMe = new CModelInstance();
	returnMe->SetModel(aModelID);

	myScene->AddModelInstance(returnMe);

	return returnMe;
}

CModelInstance * CModelLoader::CreateInstance(const char* aModelPath)
{
	CModelInstance* returnMe = new CModelInstance();

	returnMe->SetModel(CreateModel(aModelPath, "../Assets/Shaders/PS_PBLModel.hlsl", "../Assets/Shaders/PS_PBLModel.hlsl"));
	myScene->AddModelInstance(returnMe);

	return returnMe;
}


bool CModelLoader::CreateModel2(CModel& aModelOut, const char * aModelPath, const char* aVSShader, const char* aPSShader)
{
	std::string modPath(aModelPath);

	DL_WRITELOG(PURPLE,eDebugLogType::Resource, "%s%s", "[ModelLoader] - Creating Model: ", aModelPath);
	std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
	SModelData modelDataFromLoader;
	CLoaderModel* loaderModel = myLoader->LoadModel(aModelPath);

	if (loaderModel && loaderModel->myMeshes.size() > 0)
	{
		modelDataFromLoader.myIndexBufferSize = (unsigned int)loaderModel->myMeshes[0]->myIndexes.size() * sizeof(unsigned int);
		modelDataFromLoader.myIndexCount = (unsigned int)loaderModel->myMeshes[0]->myIndexes.size();
		modelDataFromLoader.myIndexes = &loaderModel->myMeshes[0]->myIndexes[0];
		modelDataFromLoader.myVertexBufferSize = loaderModel->myMeshes[0]->myVertexBufferSize;
		modelDataFromLoader.myVertexCount = loaderModel->myMeshes[0]->myVertexCount;
		modelDataFromLoader.myVertexes = loaderModel->myMeshes[0]->myVerticies;
		modelDataFromLoader.myShaderType = loaderModel->myMeshes[0]->myShaderType;
		modelDataFromLoader.myScene = (void*)loaderModel->myScene;
		modelDataFromLoader.myTextures = loaderModel->myTextures;
	}
	else
	{
		return false;
	}

	//if (loaderModel->myScene->mMeshes[0]->mNumBones > 0)
	//{
	//	aModelOut.SetIsAnimated(aModelOut.InitiateSkeleton(loaderModel->myScene));
	//}

	if (myAnimationLoader.LoadAnimations(aModelPath, aModelOut))
	{
		aModelOut.SetIsAnimated(true);
		aVSShader = "../assets/shaders/VS_DEFModelAnimatedDefault.hlsl";
		aPSShader = "../assets/shaders/PS_DEFModelAnimatedDefault.hlsl";
	}
	else
	{
		aModelOut.SetIsAnimated(false);
	}
	

	HRESULT result;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0 , DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	long long verShaderID = myShaderManager.LoadShader(aVSShader, layout, 7);
	long long pixShaderID = myShaderManager.LoadShader(aPSShader, nullptr, 0);
	aModelOut.SetShaders(verShaderID, pixShaderID);
	

	//Create triangle vertices

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.ByteWidth = modelDataFromLoader.myVertexBufferSize * modelDataFromLoader.myVertexCount;
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA bufferSubResourceData = { 0 };
	bufferSubResourceData.pSysMem = modelDataFromLoader.myVertexes;

	ID3D11Buffer* vertexBuffer = nullptr;
	result = myDevice->CreateBuffer(&bufferDescription, &bufferSubResourceData, &vertexBuffer);
	result = myDevice->GetDeviceRemovedReason();
	if ((FAILED(result) || S_FALSE == result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - vertexbuffer failed to load! Defaulting to cube! Reason: ");
		if (result == E_OUTOFMEMORY)
		{
			DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - model took up too much memory");
		}
		else if (result == E_INVALIDARG)
		{
			DL_ERROR(eDebugLogType::Resource, "%s%s", "[ModelLoader] - something went wrong with the input arguments with the following file: ", modPath.c_str());
		}
		else if (result == S_FALSE)
		{
			DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - Alternate success value, what you did worked but it was not completely correct");
		}
		myFailedModelPaths.Add(modPath);
		return false;
	}

	//Create index buffer

	D3D11_BUFFER_DESC indexBufferDescription = {};
	indexBufferDescription.ByteWidth = modelDataFromLoader.myIndexBufferSize;
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferSubResourceData = { 0 };
	indexBufferSubResourceData.pSysMem = modelDataFromLoader.myIndexes;

	ID3D11Buffer* indexBuffer;
	result = myDevice->CreateBuffer(&indexBufferDescription, &indexBufferSubResourceData, &indexBuffer);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s%s", "[ModelLoader] - indexbuffer failed to load for the following file: ", "defaulting to Cube!"/*, modPath.c_str()*/);
		myFailedModelPaths.Add(modPath);
		return false;
	}

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - InputLayout failed to load! Defaulting to cube!");
		myFailedModelPaths.Add(modPath);
		return false;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "[ModelLoader] - InputLayout load succeded!");
	}

	//Get TargetFolder
	std::string filePath = std::string(aModelPath);

	const size_t last_slash_idx = filePath.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		filePath.erase(last_slash_idx + 1, filePath.back());
	}
	//end Get TargetFolder
	//Load Albedo
	std::string TexturePath = filePath + modelDataFromLoader.myTextures[eAlbedo];
	ID3D11ShaderResourceView* texture = LoadTexture(TexturePath, eAlbedo);

	//Load NormalMap
	TexturePath = filePath + modelDataFromLoader.myTextures[eNormals];
	ID3D11ShaderResourceView* normalMap = LoadTexture(TexturePath, eNormals);
	//End load normal map

	//Load Roughness
	TexturePath = filePath + modelDataFromLoader.myTextures[eRoughness];
	ID3D11ShaderResourceView* roughness = LoadTexture(TexturePath, eRoughness);
	//End load Roughness

	//Load AmbientOcclusion
	TexturePath = filePath + modelDataFromLoader.myTextures[eAmbient];
	ID3D11ShaderResourceView* ambient = LoadTexture(TexturePath, eAmbient);
	//End load AmbientOcclusion

	//Load Emissive
	TexturePath = filePath + modelDataFromLoader.myTextures[eEmissive];
	ID3D11ShaderResourceView* emissive = LoadTexture(TexturePath, eEmissive);
	//End load Emissive

	//Load MetalNess
	TexturePath = filePath + modelDataFromLoader.myTextures[eMetalness];
	ID3D11ShaderResourceView* metalness = LoadTexture(TexturePath, eMetalness);
	//End load MetalNess

	//start setting modelData
	CModel::SModelData modelData;

	//VertexData
	modelData.myVertexData.myNumberOfVertices	= modelDataFromLoader.myVertexCount;
	modelData.myVertexData.myNumberOfIndices	= modelDataFromLoader.myIndexCount;
	modelData.myVertexData.myStride				= modelDataFromLoader.myVertexBufferSize;
	modelData.myVertexData.myOffset				= 0;
	modelData.myVertexData.myVertexBuffer		= vertexBuffer;
	modelData.myVertexData.myIndexBuffer		= indexBuffer;
	//TextureData
	modelData.myTextureData.myAlbedo			= texture;
	modelData.myTextureData.myNormalMap			= normalMap;
	modelData.myTextureData.myAmbientMap		= ambient;
	modelData.myTextureData.myRoughnessMap		= roughness;
	modelData.myTextureData.myEmissiveMap		= emissive;
	modelData.myTextureData.myMetalnessMap		= metalness;
	//End ModelData


	CreateBoundingBox((aiScene*)modelDataFromLoader.myScene, aModelOut.myBoundingBox.myMax, aModelOut.myBoundingBox.myMin);

	aModelOut.Init(modelData);

	//TODO: Buffer text so that it works in threaded systems
	//std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<float> duration = endTime - startTime;
	//float durationf = duration.count();
	//DL_WRITELOG(LIGHTBLUE,eDebugLogType::Resource, "%s%s%s%f%s", "MODEL ", aModelPath , " TOOK ", durationf, "ms to Load");
	return true;
}

void CModelLoader::CreateBoundingBox(aiScene* aScene ,Corgi::Vector3<float>& boundingBoxMax, Corgi::Vector3<float>& boundingBoxMin)
{
	for (unsigned int n = 0; n < aScene->mNumMeshes; ++n)
	{
		aiMesh* fbxMesh = aScene->mMeshes[n];
		unsigned int vertexCount = fbxMesh->mNumVertices;

		for (unsigned int i = 0; i < vertexCount; i++)
		{
			if (fbxMesh->HasPositions())
			{
				aiVector3D& mVertices = fbxMesh->mVertices[i];

				if (-mVertices.x > boundingBoxMax.x)
				{
					boundingBoxMax.x = -mVertices.x;
				}
				else if (-mVertices.x < boundingBoxMin.x)
				{
					boundingBoxMin.x = -mVertices.x;
				}

				if (mVertices.y > boundingBoxMax.y)
				{
					boundingBoxMax.y = mVertices.y;
				}
				else if (mVertices.y < boundingBoxMin.y)
				{
					boundingBoxMin.y = mVertices.y;
				}

				if (mVertices.z > boundingBoxMax.z)
				{
					boundingBoxMax.z = mVertices.z;
				}
				else if (mVertices.z < boundingBoxMin.z)
				{
					boundingBoxMin.z = mVertices.z;
				}
			}
		}
	}
}

long long CModelLoader::CreateModel(const char * aModelPath, const char* aVSShader, const char* aPSShader)
{
	std::string modPath(aModelPath);
	long long IdToReturn = myLoadedModelPaths.Find(modPath);
	if (IdToReturn != -1)
	{
		return IdToReturn;
	}
	IdToReturn = myFailedModelPaths.Find(modPath);
	if (IdToReturn != -1)
	{
		return 0;
	}

	DL_WRITELOG(PURPLE, eDebugLogType::Resource, "%s%s", "[ModelLoader] - Creating Model: ", aModelPath);
	std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

	SModelData* modelDataFromLoader = new SModelData();
	CLoaderModel* loaderModel = myLoader->LoadModel(aModelPath);

	if (loaderModel)
	{
		modelDataFromLoader->myIndexBufferSize = (unsigned int)loaderModel->myMeshes[0]->myIndexes.size() * sizeof(unsigned int);
		modelDataFromLoader->myIndexCount = (unsigned int)loaderModel->myMeshes[0]->myIndexes.size();
		modelDataFromLoader->myIndexes = &loaderModel->myMeshes[0]->myIndexes[0];
		modelDataFromLoader->myScene = (void*)loaderModel->myScene;
		modelDataFromLoader->myTextures = loaderModel->myTextures;

		modelDataFromLoader->myVertexBufferSize = loaderModel->myMeshes[0]->myVertexBufferSize;
		modelDataFromLoader->myVertexCount = loaderModel->myMeshes[0]->myVertexCount;
		modelDataFromLoader->myVertexes = loaderModel->myMeshes[0]->myVerticies;
		modelDataFromLoader->myShaderType = loaderModel->myMeshes[0]->myShaderType;
	}

	HRESULT result;


	//Load Shader
	std::ifstream vsFile;
	if (aVSShader == nullptr)
	{
		vsFile.open("../Assets/Shaders/VS_DEFModel.hlsl", std::ios::binary);
	}
	else
	{
		vsFile.open(aVSShader, std::ios::binary);
	}
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	result = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	vsFile.close();
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - vertexShader failed to load! Defaulting to cube");
		myFailedModelPaths.Add(modPath);
		return 0;
	}


	std::ifstream psFile;
	if (aPSShader == nullptr)
	{
		psFile.open("../Assets/Shaders/PS_DEFModel.hlsl", std::ios::binary);
	}
	else
	{
		psFile.open(aPSShader, std::ios::binary);
	}
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	result = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
	psFile.close();
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - pixelshader failed to load! Defaulting to cube");
		myFailedModelPaths.Add(modPath);
		return 0;
	}
	//End load


	//Create triangle vertices

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.ByteWidth = modelDataFromLoader->myVertexBufferSize * modelDataFromLoader->myVertexCount;
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA bufferSubResourceData = { 0 };
	bufferSubResourceData.pSysMem = modelDataFromLoader->myVertexes;

	ID3D11Buffer* vertexBuffer;
	result = myDevice->CreateBuffer(&bufferDescription, &bufferSubResourceData, &vertexBuffer);
	if (FAILED(result) || S_FALSE == result)
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - vertexbuffer failed to load! Defaulting to cube! Reason: ");
		if (result == E_OUTOFMEMORY)
		{
			DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - model took up too much memory");
		}
		else if (result == E_INVALIDARG)
		{
			DL_ERROR(eDebugLogType::Resource, "%s%s", "[ModelLoader] - something went wrong with the input arguments with the following file: ", modPath.c_str());
		}
		else if (result == S_FALSE)
		{
			DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - Alternate success value, what you did worked but it was not completely correct");
		}
		myFailedModelPaths.Add(modPath);
		return 0;
	}

	//Create index buffer

	D3D11_BUFFER_DESC indexBufferDescription = {};
	indexBufferDescription.ByteWidth = modelDataFromLoader->myIndexBufferSize;
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferSubResourceData = { 0 };
	indexBufferSubResourceData.pSysMem = modelDataFromLoader->myIndexes;

	ID3D11Buffer* indexBuffer;
	result = myDevice->CreateBuffer(&indexBufferDescription, &indexBufferSubResourceData, &indexBuffer);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s%s", "[ModelLoader] - indexbuffer failed to load for the following file: ", "defaulting to Cube!"/*, modPath.c_str()*/);
		myFailedModelPaths.Add(modPath);
		return 0;
	}
	//Create Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0 , DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	ID3D11InputLayout* inputLayout;
	result = myDevice->CreateInputLayout(layout, 5, vsData.data(), vsData.size(), &inputLayout);
	//End creation of Layout
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - InputLayout failed to load! Defaulting to cube!");
		myFailedModelPaths.Add(modPath);
		return 0;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "[ModelLoader] - InputLayout load succeded!");
	}

	//Get TargetFolder
	std::string filePath = std::string(aModelPath);

	const size_t last_slash_idx = filePath.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		filePath.erase(last_slash_idx + 1, filePath.back());
	}
	//end Get TargetFolder

	//Load Albedo
	std::string TexturePath = filePath + modelDataFromLoader->myTextures[eAlbedo];
	ID3D11ShaderResourceView* texture = LoadTexture(TexturePath, eAlbedo);

	//Load NormalMap
	TexturePath = filePath + modelDataFromLoader->myTextures[eNormals];
	ID3D11ShaderResourceView* normalMap = LoadTexture(TexturePath, eNormals);
	//End load normal map

	//Load Roughness
	TexturePath = filePath + modelDataFromLoader->myTextures[eRoughness];
	ID3D11ShaderResourceView* roughness = LoadTexture(TexturePath, eRoughness);
	//End load Roughness

	//Load AmbientOcclusion
	TexturePath = filePath + modelDataFromLoader->myTextures[eAmbient];
	ID3D11ShaderResourceView* ambient = LoadTexture(TexturePath, eAmbient);
	//End load AmbientOcclusion

	//Load Emissive
	TexturePath = filePath + modelDataFromLoader->myTextures[eEmissive];
	ID3D11ShaderResourceView* emissive = LoadTexture(TexturePath, eEmissive);
	//End load Emissive

	//Load MetalNess
	TexturePath = filePath + modelDataFromLoader->myTextures[eMetalness];
	ID3D11ShaderResourceView* metalness = LoadTexture(TexturePath, eEmissive);
	//End load MetalNess

	//start setting modelData
	CModel::SModelData modelData;
	//VertexData
	modelData.myVertexData.myNumberOfVertices = modelDataFromLoader->myVertexCount;
	modelData.myVertexData.myNumberOfIndices = modelDataFromLoader->myIndexCount;
	modelData.myVertexData.myStride = modelDataFromLoader->myVertexBufferSize;
	modelData.myVertexData.myOffset = 0;
	modelData.myVertexData.myVertexBuffer = vertexBuffer;
	modelData.myVertexData.myIndexBuffer = indexBuffer;
	//ShaderData
	modelData.myShaderData.myVertexShader = vertexShader;
	modelData.myShaderData.myPixelShader = pixelShader;
	//LayoutData
	modelData.myLayoutData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myLayoutData.myInputLayout = inputLayout;
	//TextureData
	modelData.myTextureData.myAlbedo = texture;
	modelData.myTextureData.myNormalMap = normalMap;
	modelData.myTextureData.myAmbientMap = ambient;
	modelData.myTextureData.myRoughnessMap = roughness;
	modelData.myTextureData.myEmissiveMap = emissive;
	modelData.myTextureData.myMetalnessMap = metalness;
	//End ModelData

	CModel ModelToAdd;
	ModelToAdd.Init(modelData);
	myModels.Add(ModelToAdd);
	myLoadedModelPaths.Add(modPath);
	IdToReturn = myModels.Size() - 1;


	std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

	std::chrono::duration<float> duration = endTime - startTime;
	float durationf = duration.count();

	DL_WRITELOG(LIGHTBLUE, eDebugLogType::Resource, "%s%s%s%f%s", "[ModelLoader] - MODEL ", aModelPath, " TOOK ", durationf, "ms to Load");
	return IdToReturn;
}

bool CModelLoader::CreateSkybox2(CModel& aModelOut, const char * aTexturePath)
{
	Vertex vertices[24] =
	{
		//Front
		{ -1.0f	, -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 1.0f },
		{ -1.0f	, 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 0.0f },
		{ 1.0f	, 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 0.0f },
		{ 1.0f	, -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 1.0f },
		//Right
		{ 1.0f	, -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 1.0f },
		{ 1.0f	, 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 0.0f },
		{ 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 0.0f },
		{ 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 1.0f },
		//Back
		{ 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 1.0f },
		{ 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 0.0f },
		{ -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 0.0f },
		{ -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 1.0f },
		//Left
		{ -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 1.0f },
		{ -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 0.0f },
		{ -1.0f	, 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 0.0f },
		{ -1.0f	, -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 1.0f },
		//Top
		{ -1.0f	, 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 1.0f },
		{ -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 0.0f },
		{ 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 0.0f },
		{ 1.0f	, 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 1.0f },
		//Bottom
		{ -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 1.0f },
		{ -1.0f	, -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 0.0f },
		{ 1.0f	, -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 0.0f },
		{ 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 1.0f },
	};
	unsigned int indices[36]
	{
		//Front
		2,1,0,
		3,2,0,
		//Right
		6,5,4,
		7,6,4,
		//Back
		10,9,8,
		11,10,8,
		//Left
		14,13,12,
		15,14,12,
		//Top
		18,17,16,
		19,18,16,
		//Bottom
		22,21,20,
		23,22,20,
	};

	HRESULT result;
	//Load Shader
	//std::ifstream vsFile;
	//vsFile.open("../Assets/Shaders/VS_Skybox.hlsl", std::ios::binary);
	//std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	//vsFile.close();

	ID3D10Blob* VSBlob = nullptr;
	ID3D10Blob* PSBlob = nullptr;

	myShaderManager.GetShaderCompiler().CompileShader("../Assets/Shaders/VS_Skybox.hlsl", "main", "vs_5_0", VSBlob);
	myShaderManager.GetShaderCompiler().CompileShader("../Assets/Shaders/PS_Skybox.hlsl", "main", "ps_5_0", PSBlob);

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	result = myDevice->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &vertexShader);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Shader, "%s", "Error creating Skybox2 Vertexshader ");
	}
	result = myDevice->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &pixelShader);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Shader, "%s", "Error creating Skybox2 Pixelshader ");
	}
	//std::ifstream psFile;
	//psFile.open("../Assets/Shaders/PS_Skybox.hlsl", std::ios::binary);
	//std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	//psFile.close();

	//End load

	//Create triangle vertices

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.ByteWidth = sizeof(vertices);
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA bufferSubResourceData = { 0 };
	bufferSubResourceData.pSysMem = vertices;

	ID3D11Buffer* vertexBuffer;
	result = myDevice->CreateBuffer(&bufferDescription, &bufferSubResourceData, &vertexBuffer);


	//Create index buffer

	D3D11_BUFFER_DESC indexBufferDescription = {};
	indexBufferDescription.ByteWidth = sizeof(indices);
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferSubResourceData = { 0 };
	indexBufferSubResourceData.pSysMem = indices;

	ID3D11Buffer* indexBuffer;
	result = myDevice->CreateBuffer(&indexBufferDescription, &indexBufferSubResourceData, &indexBuffer);

	//Create Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0 , DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	ID3D11InputLayout* inputLayout;
	result = myDevice->CreateInputLayout(layout, 3, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &inputLayout);
	//End creation of Layout
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - Create skybox InputLayout failed to load");
		return false;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "[ModelLoader] - Create skybox InputLayout load succeded");
	}

	//Create and return(?) model
	CModel::SModelData modelData;
	modelData.myVertexData.myNumberOfVertices = sizeof(vertices) / sizeof(Vertex);
	modelData.myVertexData.myNumberOfIndices = sizeof(indices) / sizeof(UINT);
	modelData.myVertexData.myStride = sizeof(Vertex);
	modelData.myVertexData.myOffset = 0;
	modelData.myVertexData.myVertexBuffer = vertexBuffer;
	modelData.myVertexData.myIndexBuffer = indexBuffer;
	modelData.myShaderData.myVertexShader = vertexShader;
	modelData.myShaderData.myPixelShader = pixelShader;
	modelData.myLayoutData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myLayoutData.myInputLayout = inputLayout;

	std::string path = aTexturePath;
	modelData.myTextureData.myAlbedo = LoadTexture(path, eAlbedo);

	aModelOut.Init(modelData);
	return true;
}

long long CModelLoader::CreateSkybox(const char * aTexturePath)
{
	long long IdToReturn = myLoadedModelPaths.Find("skybox");
	if (IdToReturn != -1)
	{
		return IdToReturn;
	}

	Vertex vertices[24] =
	{
		//Front
		{ -1.0f	, -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 1.0f },
		{ -1.0f	, 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 0.0f },
		{ 1.0f	, 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 0.0f },
		{ 1.0f	, -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 1.0f },
		//Right
		{ 1.0f	, -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 1.0f },
		{ 1.0f	, 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 0.0f },
		{ 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 0.0f },
		{ 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 1.0f },
		//Back
		{ 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 1.0f },
		{ 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 0.0f },
		{ -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 0.0f },
		{ -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 1.0f },
		//Left
		{ -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 1.0f },
		{ -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 0.0f },
		{ -1.0f	, 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 0.0f },
		{ -1.0f	, -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 1.0f },
		//Top
		{ -1.0f	, 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 1.0f },
		{ -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 0.0f },
		{ 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 0.0f },
		{ 1.0f	, 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 1.0f },
		//Bottom
		{ -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 1.0f },
		{ -1.0f	, -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 0.0f, 0.0f },
		{ 1.0f	, -1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 0.0f },
		{ 1.0f	, -1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f	, 1.0f, 1.0f },
	};
	unsigned int indices[36]
	{
		//Front
		2,1,0,
		3,2,0,
		//Right
		6,5,4,
		7,6,4,
		//Back
		10,9,8,
		11,10,8,
		//Left
		14,13,12,
		15,14,12,
		//Top
		18,17,16,
		19,18,16,
		//Bottom
		22,21,20,
		23,22,20,
	};
	
	HRESULT result;
	//Load Shader
	std::ifstream vsFile;
	vsFile.open("../Assets/Shaders/VS_Skybox.hlsl", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	result = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	vsFile.close();

	std::ifstream psFile;
	psFile.open("../Assets/Shaders/PS_Skybox.hlsl", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	result = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
	psFile.close();

	//End load

	//Create triangle vertices

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.ByteWidth = sizeof(vertices);
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA bufferSubResourceData = { 0 };
	bufferSubResourceData.pSysMem = vertices;

	ID3D11Buffer* vertexBuffer;
	result = myDevice->CreateBuffer(&bufferDescription, &bufferSubResourceData, &vertexBuffer);


	//Create index buffer

	D3D11_BUFFER_DESC indexBufferDescription = {};
	indexBufferDescription.ByteWidth = sizeof(indices);
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferSubResourceData = { 0 };
	indexBufferSubResourceData.pSysMem = indices;

	ID3D11Buffer* indexBuffer;
	result = myDevice->CreateBuffer(&indexBufferDescription, &indexBufferSubResourceData, &indexBuffer);

	//Create Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0 , DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	ID3D11InputLayout* inputLayout;
	result = myDevice->CreateInputLayout(layout, 3, vsData.data(), vsData.size(), &inputLayout);
	//End creation of Layout
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - Create skybox InputLayout failed to load");
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "[ModelLoader] - Create skybox InputLayout load succeded");
	}

	//Create and return(?) model
	CModel::SModelData modelData;
	modelData.myVertexData.myNumberOfVertices = sizeof(vertices) / sizeof(Vertex);
	modelData.myVertexData.myNumberOfIndices = sizeof(indices) / sizeof(UINT);
	modelData.myVertexData.myStride = sizeof(Vertex);
	modelData.myVertexData.myOffset = 0;
	modelData.myVertexData.myVertexBuffer = vertexBuffer;
	modelData.myVertexData.myIndexBuffer = indexBuffer;
	modelData.myShaderData.myVertexShader = vertexShader;
	modelData.myShaderData.myPixelShader = pixelShader;
	modelData.myLayoutData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myLayoutData.myInputLayout = inputLayout;

	std::string path = aTexturePath;
	modelData.myTextureData.myAlbedo = LoadTexture(path, eAlbedo);

	CModel returnMe;
	returnMe.Init(modelData);
	myModels.Add(returnMe);
	myLoadedModelPaths.Add("skybox");

	DL_SUCCESS(eDebugLogType::Resource, "%s", "[ModelLoader] - skybox succeded on load");

	return myModels.Size() - 1;
}

bool CModelLoader::CreateCube2(CModel& aModelOut)
{

	Vertex vertices[24] =
	{
		//Front
		{ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		//Right
		{ 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		//Back
		{ 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		//Left
		{ -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		//Top
		{ -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		//Bottom
		{ -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	};
	unsigned int indices[36]
	{
		//Front
		0,1,2,
		0,2,3,
		//Right
		4,5,6,
		4,6,7,
		//Back
		8,9,10,
		8,10,11,
		//Left
		12,13,14,
		12,14,15,
		//Top
		16,17,18,
		16,18,19,
		//Bottom
		20,21,22,
		20,22,23,
	};

	HRESULT result;
	//Create triangle vertices

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0 , DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	const char* VSShader = "../assets/shaders/VS_DEFModelDefault.hlsl";
	const char* PSShader = "../assets/shaders/PS_DEFModelDefault.hlsl";

	long long verShaderID = myShaderManager.LoadShader(VSShader, layout, 7);
	long long pixShaderID = myShaderManager.LoadShader(PSShader, nullptr, 0);
	aModelOut.SetShaders(verShaderID, pixShaderID);

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.ByteWidth = sizeof(vertices);
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA bufferSubResourceData = { 0 };
	bufferSubResourceData.pSysMem = vertices;

	ID3D11Buffer* vertexBuffer;
	result = myDevice->CreateBuffer(&bufferDescription, &bufferSubResourceData, &vertexBuffer);


	//Create index buffer

	D3D11_BUFFER_DESC indexBufferDescription = {};
	indexBufferDescription.ByteWidth = sizeof(indices);
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferSubResourceData = { 0 };
	indexBufferSubResourceData.pSysMem = indices;

	ID3D11Buffer* indexBuffer;
	result = myDevice->CreateBuffer(&indexBufferDescription, &indexBufferSubResourceData, &indexBuffer);

	//Create and return(?) model
	CModel::SModelData modelData;
	modelData.myVertexData.myNumberOfVertices = sizeof(vertices) / sizeof(Vertex);
	modelData.myVertexData.myNumberOfIndices = sizeof(indices) / sizeof(UINT);
	modelData.myVertexData.myStride = sizeof(Vertex);
	modelData.myVertexData.myOffset = 0;
	modelData.myVertexData.myVertexBuffer = vertexBuffer;
	modelData.myVertexData.myIndexBuffer = indexBuffer;

	std::string textureFilename = "ERRORCUBE";
	modelData.myTextureData.myAlbedo = LoadTexture(textureFilename, eAlbedo);
	modelData.myTextureData.myNormalMap = LoadTexture(textureFilename, eNormals);
	modelData.myTextureData.myAmbientMap = LoadTexture(textureFilename, eAmbient);
	modelData.myTextureData.myRoughnessMap = LoadTexture(textureFilename, eRoughness);
	modelData.myTextureData.myEmissiveMap = LoadTexture(textureFilename, eEmissive);
	modelData.myTextureData.myMetalnessMap = LoadTexture(textureFilename, eMetalness);

	aModelOut.Init(modelData);

	return true;
}
long long CModelLoader::CreateCube()
{
	long long IdToReturn = myLoadedModelPaths.Find("cube");
	if (IdToReturn != -1)
	{
		return IdToReturn;
	}

	Vertex vertices[24] =
	{
		//Front
		{ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		//Right
		{ 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		//Back
		{ 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		//Left
		{ -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		//Top
		{ -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
		//Bottom
		{ -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f },
		{ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
	};
	unsigned int indices[36]
	{
		//Front
		0,1,2,
		0,2,3,
		//Right
		4,5,6,
		4,6,7,
		//Back
		8,9,10,
		8,10,11,
		//Left
		12,13,14,
		12,14,15,
		//Top
		16,17,18,
		16,18,19,
		//Bottom
		20,21,22,
		20,22,23,
	};

	HRESULT result;

	ID3D10Blob* VSBlob = nullptr;
	ID3D10Blob* PSBlob = nullptr;

	myShaderManager.GetShaderCompiler().CompileShader("../Assets/Shaders/VS_PBLModel.hlsl", "main", "vs_5_0", VSBlob);
	myShaderManager.GetShaderCompiler().CompileShader("../Assets/Shaders/PS_PBLModel.hlsl", "main", "ps_5_0", PSBlob);

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	result = myDevice->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &vertexShader);
	result = myDevice->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &pixelShader);


	//Create triangle vertices

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.ByteWidth = sizeof(vertices);
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA bufferSubResourceData = { 0 };
	bufferSubResourceData.pSysMem = vertices;

	ID3D11Buffer* vertexBuffer;
	result = myDevice->CreateBuffer(&bufferDescription, &bufferSubResourceData, &vertexBuffer);


	//Create index buffer

	D3D11_BUFFER_DESC indexBufferDescription = {};
	indexBufferDescription.ByteWidth = sizeof(indices);
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferSubResourceData = { 0 };
	indexBufferSubResourceData.pSysMem = indices;

	ID3D11Buffer* indexBuffer;
	result = myDevice->CreateBuffer(&indexBufferDescription, &indexBufferSubResourceData, &indexBuffer);

	//Create Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0 , DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	ID3D11InputLayout* inputLayout;
	result = myDevice->CreateInputLayout(layout, 5, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &inputLayout);
	//End creation of Layout
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - Create Cube InputLayout failed to load");
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "[ModelLoader] - Create Cube InputLayout load succeded");
	}

	//Create and return(?) model
	CModel::SModelData modelData;
	modelData.myVertexData.myNumberOfVertices = sizeof(vertices) / sizeof(Vertex);
	modelData.myVertexData.myNumberOfIndices = sizeof(indices) /sizeof(UINT);
	modelData.myVertexData.myStride = sizeof(Vertex);
	modelData.myVertexData.myOffset = 0;
	modelData.myVertexData.myVertexBuffer = vertexBuffer;
	modelData.myVertexData.myIndexBuffer = indexBuffer;
	modelData.myShaderData.myVertexShader = vertexShader;
	modelData.myShaderData.myPixelShader = pixelShader;
	modelData.myLayoutData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myLayoutData.myInputLayout = inputLayout;

	std::string textureFilename = "ERRORCUBE";
	modelData.myTextureData.myAlbedo = LoadTexture(textureFilename, eAlbedo);
	modelData.myTextureData.myNormalMap = LoadTexture(textureFilename, eNormals);
	modelData.myTextureData.myAmbientMap = LoadTexture(textureFilename, eAmbient);
	modelData.myTextureData.myRoughnessMap = LoadTexture(textureFilename, eRoughness);
	modelData.myTextureData.myEmissiveMap = LoadTexture(textureFilename, eEmissive);
	modelData.myTextureData.myMetalnessMap = LoadTexture(textureFilename, eMetalness);

	CModel returnMe;
	returnMe.Init(modelData);
	myModels.Add(returnMe);
	myLoadedModelPaths.Add("cube");


	DL_SUCCESS(eDebugLogType::Resource, "%s", "Cube succeded on load");

	VSBlob->Release();
	VSBlob = 0;
	PSBlob->Release();
	PSBlob = 0;

	return myModels.Size() - 1;
}

long long CModelLoader::CreateSphere()
{
	return 0;
}

long long CModelLoader::CreateTriangle()
{
	long long IdToReturn = myLoadedModelPaths.Find("triangle");
	if (IdToReturn != -1)
	{
		return IdToReturn;
	}

	HRESULT result;
	//Load Shader
	std::ifstream vsFile;
	vsFile.open("../Assets/Shaders/VS_PBLSprite.hlsl", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	result = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	vsFile.close();

	std::ifstream psFile;
	psFile.open("../Assets/Shaders/PS_PBLSprite.hlsl", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	result = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
	psFile.close();

	//End load

	//Create triangle vertices
	Vertex vertices[3] =
	{
		{ -0.8f, -0.8f, 0, 1, 1, 0, 0, 1, 1, 1 },
		{ -0.8f, -0.8f, 0, 1, 1, 0, 0, 1, 1, 1 },
		{ -0.8f, -0.8f, 0, 1, 1, 0, 0, 1, 1, 1 }
	};
	unsigned int indices[3] = {	0,1,2 };

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.ByteWidth = sizeof(vertices);
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA bufferSubResourceData = { 0 };
	bufferSubResourceData.pSysMem = vertices;

	ID3D11Buffer* vertexBuffer;
	result = myDevice->CreateBuffer(&bufferDescription, &bufferSubResourceData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDescription = {};
	indexBufferDescription.ByteWidth = sizeof(indices);
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexBufferSubResourceData = { 0 };
	indexBufferSubResourceData.pSysMem = indices;

	ID3D11Buffer* indexBuffer;
	result = myDevice->CreateBuffer(&indexBufferDescription, &indexBufferSubResourceData, &indexBuffer);

	//Create Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0 , DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ID3D11InputLayout* inputLayout;
	result = myDevice->CreateInputLayout(layout, 3, vsData.data(), vsData.size(), &inputLayout);
	//End creation of Layout
	//Texture
	std::wstring filename = L"../Assets/Textures/New_Graph_basecolor.dds";
	ID3D11ShaderResourceView* texture;
	result = DirectX::CreateDDSTextureFromFile(myDevice, filename.c_str(), nullptr, &texture);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - Texture failed to load");
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s", "[ModelLoader] - Texture load succeded");
	}
	//end load texture
	//Create and return(?) model

	CModel::SModelData modelData;
	modelData.myVertexData.myNumberOfVertices = sizeof(vertices) / sizeof(Vertex);
	modelData.myVertexData.myNumberOfIndices = sizeof(indices) / sizeof(UINT);
	modelData.myVertexData.myStride = sizeof(Vertex);
	modelData.myVertexData.myOffset = 0;
	modelData.myVertexData.myVertexBuffer = vertexBuffer;
	modelData.myVertexData.myIndexBuffer = indexBuffer;
	modelData.myShaderData.myVertexShader = vertexShader;
	modelData.myShaderData.myPixelShader = pixelShader;
	modelData.myLayoutData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myLayoutData.myInputLayout = inputLayout;
	modelData.myTextureData.myAlbedo = texture;

	CModel returnMe;
	returnMe.Init(modelData);
	myModels.Add(returnMe);
	myLoadedModelPaths.Add("triangle");
	return myModels.Size() - 1;
}

void CModelLoader::AssignScene(CScene * aScene)
{
	myScene = aScene;
}

ID3D11ShaderResourceView* CModelLoader::LoadTexture(std::string& aFilePath, int aType)
{

	std::wstring filename;
	filename.assign(aFilePath.begin(), aFilePath.end());
	ID3D11ShaderResourceView * creation;
	HRESULT result = DirectX::CreateDDSTextureFromFile(myDevice, filename.c_str(), nullptr, &creation);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s%s", "[ModelLoader] - LoadTexture failed to load file: ", aFilePath.c_str());
		if (result == D3D11_ERROR_FILE_NOT_FOUND)
		{
			DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - Texture File was not found");
		}
		if (aType == eEmissive || aType == eMetalness || aType == eRoughness)
		{
			DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - texture was set to default Full black!");
			creation = myFullBlackErrorTexture;
		}
		else if (aType == eAmbient )
		{
			DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - texture was set to default white texture!");
			creation = myFullWhiteErrorTexture;
		}
		else
		{
			DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - defaulting to chequered error sprite!");
			creation = myChequeredErrorTexture;
		}
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Resource, "%s%s%s", "[ModelLoader] - Texture ", aFilePath.c_str() , " load succeded");
	}
	return creation;
}


ID3D11ShaderResourceView* CModelLoader::CreateDefaultTexture(bool aIsChequered, int aErrorTextureColor)
{
	HRESULT result;
	ID3D11Texture2D *tex;
	D3D11_TEXTURE2D_DESC tdesc;
	D3D11_SUBRESOURCE_DATA tbsd;

	int *buf;
	int h;
	int w;
	if (aIsChequered == true)
	{
		h = 16;
		w = 16;
		buf = new int[h*w];
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				if ((i + j) % 2 == 0)
				{
					buf[i*w + j] = 0xff000000;
				}
				else
				{
					buf[i*w + j] = aErrorTextureColor;
				}
			}
		}
	}
	else
	{
		h = 4;
		w = 4;
		buf = new int[h*w];
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				buf[i*w + j] = aErrorTextureColor;
			}
		}
	}

	tbsd.pSysMem = (void *)buf;
	tbsd.SysMemPitch = w * 4;
	tbsd.SysMemSlicePitch = w*h * 4; // Not needed since this is a 2d texture

	tdesc.Width = w;
	tdesc.Height = h;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 1;

	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	tdesc.CPUAccessFlags = 0;
	tdesc.MiscFlags = 0;

	if (FAILED(myDevice->CreateTexture2D(&tdesc, &tbsd, &tex)))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - Failed to create default error texture");
		return nullptr;
	}

	delete[] buf;
	ID3D11ShaderResourceView* loadingErrorTexture = nullptr;

	result = myDevice->CreateShaderResourceView(tex, NULL, &loadingErrorTexture);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "[ModelLoader] - Failed to create default error texture");
		return nullptr;
	}
	tex->Release();
	return loadingErrorTexture;
}


int CModelLoader::GetAmountOfModels()
{
	return static_cast<int>(myModels.Size());
}

CModel & CModelLoader::GetModel(long long aID)
{
	return myModels[aID];
}
