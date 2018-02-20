#include "stdafx.h"
#ifndef _RETAIL //We dont use debugdrawer in retail environment



#include "Lines.h"
#include "psapi.h"
#include "../Script/ScriptManager.h"

#define MAXAMOUNTOFPOINTSONALINE 2048

int CDebugDrawer::DrawArrowFromLua(lua_State * aState)
{
	Vector4f myFromPosition;
	Vector4f myToPosition;

	int args = lua_gettop(aState) + 1;
	if (args > 7)
	{
		DL_WARNING(eDebugLogType::Script, "%s", "Too many arguments in DrawArrow");
		return 1;
	}

	if (lua_isnumber(aState, 1))
	{
		 myFromPosition.x = cast_f(lua_tonumber(aState, 1));
	}
	if (lua_isnumber(aState, 2))
	{
		myFromPosition.y = cast_f(lua_tonumber(aState, 2));
	}	
	if (lua_isnumber(aState, 3))
	{
		myFromPosition.z = cast_f(lua_tonumber(aState, 3));
	}	
	if (lua_isnumber(aState, 4))
	{
		myToPosition.x = cast_f(lua_tonumber(aState, 4));
	}	
	if (lua_isnumber(aState, 5))
	{
		myToPosition.y = cast_f(lua_tonumber(aState, 5));
	}
	if (lua_isnumber(aState, 6))
	{
		myFromPosition.z = cast_f(lua_tonumber(aState, 6));
	}
	myFromPosition.w = 1;
	myToPosition.w = 1;	
	CLines line;
	line.Init(MAXAMOUNTOFPOINTSONALINE);
	SetUpArrow(line, myFromPosition, myToPosition);

	//std::lock_guard<mutex> safe(myLineMutex);
	myLuaLines.Add(line);
	return 0;
}

int CDebugDrawer::DrawCubeFromLua(lua_State * aState)
{
	Vector3f myPosition;
	int args = lua_gettop(aState) + 1;
	if (args > 4)
	{
		DL_WARNING(eDebugLogType::Script, "%s", "Too many arguments in DrawCube");
		return 1;
	}

	if (lua_isnumber(aState, 1))
	{
		myPosition.x = cast_f(lua_tonumber(aState, 1));
	}
	if (lua_isnumber(aState, 2))
	{
		myPosition.y = cast_f(lua_tonumber(aState, 2));
	}
	if (lua_isnumber(aState, 3))
	{
		myPosition.z = cast_f(lua_tonumber(aState, 3));
	}


	DrawCube(myPosition);
	return 0;
}

int CDebugDrawer::DrawLineFromLua(lua_State * aState)
{
	Vector4f myFromPosition;
	Vector4f myToPosition;
	
	int args = lua_gettop(aState) + 1;
	if (args > 7)
	{
		DL_WARNING(eDebugLogType::Script, "%s", "Too many arguments in DrawLine");
		return 1;
	}

	if (lua_isnumber(aState, 1))
	{
		myFromPosition.x = cast_f(lua_tonumber(aState, 1));
	}
	if (lua_isnumber(aState, 2))
	{
		myFromPosition.y = cast_f(lua_tonumber(aState, 2));
	}
	if (lua_isnumber(aState, 3))
	{
		myFromPosition.z = cast_f(lua_tonumber(aState, 3));
	}
	if (lua_isnumber(aState, 4))
	{
		myToPosition.x = cast_f(lua_tonumber(aState, 4));
	}
	if (lua_isnumber(aState, 5))
	{
		myToPosition.y = cast_f(lua_tonumber(aState, 5));
	}
	if (lua_isnumber(aState, 6))
	{
		myFromPosition.z = cast_f(lua_tonumber(aState, 6));
	}
	myFromPosition.w = 1;
	myToPosition.w = 1;

	if (myEngine->GetGlobalCamera()->isPointInViewSpace(myFromPosition) || myEngine->GetGlobalCamera()->isPointInViewSpace(myToPosition))
	{
		CLines line;
		line.Init(MAXAMOUNTOFPOINTSONALINE);
		line.AddPoint(myFromPosition);
		line.AddPoint(myToPosition);

		//std::lock_guard<mutex> safe(myLineMutex);
		myLuaLines.Add(line);
	}
	return 0;
}

int CDebugDrawer::PrintTextFromLua(lua_State * aState)
{
	int args = lua_gettop(aState) + 1;

	std::stringstream ss("");

	for (int i = 1; i < args; ++i)
	{

		if (lua_isinteger(aState, i))
		{
			ss << lua_tointeger(aState, i);
		}
		else if (lua_isnumber(aState, i))
		{
			ss << lua_tonumber(aState, i);
		}
		else if (lua_isstring(aState, i))
		{
			ss << lua_tostring(aState, i);
		}
		else if (lua_isboolean(aState, i))
		{
			switch (lua_toboolean(aState, i))
			{
			case 0:
				ss << "false";
				break;
			case 1:
				ss << "true";
				break;
			}
		}
		ss << " ";
	}

	myDebugTextFromLua = ss.str();
	return 0;
}

CDebugDrawer::CDebugDrawer()
{

}


CDebugDrawer::~CDebugDrawer()
{


	mySingleLineInputLayout->Release();
	mySingleLineVertexShader->Release();
	mySingleLinePixelShader->Release();
	myViewPositionBuffer->Release();
	myInstanceBuffer->Release();
	myLineVertexBuffer->Release();
}

void CDebugDrawer::Init(CEngine * aEngine)
{
	myCPUUseReader.Init();
	myDebugInformationTimer = 0.f;
	myEngine = aEngine;
	myCubeID = myEngine->GetModelLoader().CreateCube();
	mySphereID = myEngine->GetModelLoader().CreateSphere();
	myCubes.Init(256);
	myLuaLines.Init(256);

	for (int Index = 0; Index < 256; Index++)
	{
		myCubes[Index] = myEngine->CreateInstance(myCubeID);
		myCubes[Index]->SetShouldRender(false);
	}
	mySpheres.Init(256);
	for (int Index = 0; Index < 256; Index++)
	{
		mySpheres[Index] = myEngine->CreateInstance(mySphereID);
		mySpheres[Index]->SetShouldRender(false);
	}
	myDebugLines.Init(MAXAMOUNTOFPOINTSONALINE);
	for (int Index = 0; Index < MAXAMOUNTOFPOINTSONALINE; Index++)
	{
		myDebugLines[Index].Init(0);
	}
	myDebugText = myEngine->GetTextManager().AddText(myDebugString, { -1.f,0.5f }, 0);
	myEngine->GetTextManager().AddText(myDebugTextFromLua, { 0.f,0.5f }, 0);

	CreateShaders();
	CreateViewPositionBuffer();
	CreateInstanceBuffer();
	CreateLineVertexBuffer();


}

void CDebugDrawer::EndFrame()
{
	for (int i = 0; i < myAmountOfActiveCubes; i++)
	{
		myCubes[i]->SetShouldRender(false);
	}
	for (int i = 0; i < myAmountOfActiveCubes; i++)
	{
		mySpheres[i]->SetShouldRender(false);
	}
	myDebugLines.FreeAll();
	myAmountOfActiveLines = 0;
	myCubes.FreeAll();
	myAmountOfActiveCubes = 0;
	mySpheres.FreeAll();
	myAmountOfActiveSpheres = 0;
}

CModelInstance * CDebugDrawer::CreateCube(const Vector3f & aPos)
{
	CModelInstance* Cube = myEngine->GetModelLoader().CreateInstance(myCubeID);
	Cube->SetPosition(aPos);
	Cube->SetShouldRender(true);
	return Cube;
}

CModelInstance * CDebugDrawer::CreateSphere(const Vector3f & aPos)
{
	CModelInstance* Sphere = myEngine->GetModelLoader().CreateInstance(mySphereID);
	Sphere->SetPosition(aPos);
	Sphere->SetShouldRender(true);
	return Sphere;
}

void CDebugDrawer::DrawCube(const Vector3f & aPos)
{
	long long freeItem = myCubes.GetFreeItem();
	myCubes[freeItem]->SetPosition(aPos);
	myCubes[freeItem]->SetShouldRender(true);
	myAmountOfActiveCubes++;
}

void CDebugDrawer::DrawSphere(const Vector3f & aPos)
{
	long long freeItem = mySpheres.GetFreeItem();
	mySpheres[freeItem]->SetPosition(aPos);
	mySpheres[freeItem]->SetShouldRender(true);
	myAmountOfActiveSpheres++;
}

void CDebugDrawer::DrawLine(const Vector4f & aFromPos, const Vector4f & aToPos)
{
	if (myEngine->GetGlobalCamera()->isPointInViewSpace(aFromPos) || myEngine->GetGlobalCamera()->isPointInViewSpace(aToPos))
	{
		long long lineIndex = myDebugLines.GetFreeItem();
		CLines& line = myDebugLines[lineIndex];
		line.Reinit(MAXAMOUNTOFPOINTSONALINE);
		line.AddPoint(aFromPos);
		line.AddPoint(aToPos);
		myAmountOfActiveLines++;
	}
}

void CDebugDrawer::DrawLine(Vector4f* aPointArray, int AmountOfPoints)
{
	long long lineIndex = myDebugLines.GetFreeItem();
	CLines& line = myDebugLines[lineIndex];
	line.Reinit(AmountOfPoints);
	for (int i = 0; i < AmountOfPoints; ++i)
	{
		line.AddPoint(aPointArray[i]);
	}
	myAmountOfActiveLines++;
}

void CDebugDrawer::DrawArrow(const Vector4f & aFromPos, const Vector4f & aToPos, const Vector3f aLineColor, const Vector3f aArrowColor, int aQuality)
{
	long long lineIndex = myDebugLines.GetFreeItem();
	CLines& line = myDebugLines[lineIndex];


	line.Reinit(MAXAMOUNTOFPOINTSONALINE);
	SetUpArrow(line, aFromPos, aToPos, aLineColor, aArrowColor, aQuality);

	myAmountOfActiveLines++;
}

const int& CDebugDrawer::GetActiveLinesSize() const
{
	return myAmountOfActiveLines;
}

void CDebugDrawer::SetUpArrow(CLines & aLine, const Vector4f & aFromPos, const Vector4f & aToPos, const Vector3f aLineColor, const Vector3f aArrowColor, int aQuality)
{
	float numberOfChevronLines = 3.0f * aQuality;
	if (numberOfChevronLines > (MAXAMOUNTOFPOINTSONALINE / 3) - 5)
	{
		numberOfChevronLines = (MAXAMOUNTOFPOINTSONALINE / 3) - 5;
	}

	aLine.AddPoint(aFromPos, aLineColor);
	aLine.AddPoint(aToPos, aLineColor);

	Vector4f oppositeDirection(aFromPos - aToPos);
	float length = oppositeDirection.Length();
	oppositeDirection.Normalize();

	Vector4f arbitraryPerpendicularLine(-oppositeDirection.y, oppositeDirection.z, oppositeDirection.x, 1.0f);

	const float chevronLength = length / 5.0f;
	const float rotationBetweenChevronLines = (PI * 2) / numberOfChevronLines;
	const float riseRotation = (PI / 2.f) / 3.0f;

	//Chevron lines
	Vector4f previousPosition = aToPos;
	for (float i = 0.0f; i < numberOfChevronLines; ++i)
	{
		aLine.AddPoint(previousPosition, aArrowColor);

		Matrix44 rotationMatrix = Matrix44::CreateRotateAroundAxis(arbitraryPerpendicularLine, riseRotation);
		Vector4f chevronLineDirection = oppositeDirection * rotationMatrix;
		rotationMatrix = Matrix44::CreateRotateAroundAxis(oppositeDirection, rotationBetweenChevronLines * i);
		chevronLineDirection = chevronLineDirection * rotationMatrix;
		chevronLineDirection.Normalize();

		previousPosition = aToPos + (chevronLineDirection * chevronLength);
		aLine.AddPoint(aToPos + (chevronLineDirection * chevronLength), aArrowColor);
		aLine.AddPoint(aToPos, aArrowColor);
	}
	aLine.AddPoint(previousPosition, aArrowColor);

	Matrix44 rotationMatrix = Matrix44::CreateRotateAroundAxis(arbitraryPerpendicularLine, riseRotation);
	Vector4f chevronLineDirection = oppositeDirection * rotationMatrix;
	rotationMatrix = Matrix44::CreateRotateAroundAxis(oppositeDirection, rotationBetweenChevronLines * (numberOfChevronLines));
	chevronLineDirection = chevronLineDirection * rotationMatrix;
	chevronLineDirection.Normalize();

	previousPosition = aToPos + (chevronLineDirection * chevronLength);
	aLine.AddPoint(aToPos + (chevronLineDirection * chevronLength), aArrowColor);
}

void CDebugDrawer::DrawDebugText(std::string aText)
{
	myTextToAddToPrint += "\n";
	myTextToAddToPrint += aText;
}

void CDebugDrawer::RenderLines(CCameraInstance* aCameraInstance)
{
	ID3D11DeviceContext* context = myEngine->GetDXFrameWork()->GetContext();
	ID3D11Buffer* cameraBuffer = aCameraInstance->GetCamera()->GetBuffer();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	context->IASetInputLayout(mySingleLineInputLayout);
	context->VSSetShader(mySingleLineVertexShader, nullptr, 0);
	context->PSSetShader(mySingleLinePixelShader, nullptr, 0);
	context->VSSetConstantBuffers(0, 1, &cameraBuffer);
	const UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &myLineVertexBuffer, &myLineStride, &offset);


	for (int i = 0; i < myAmountOfActiveLines; ++i)
	{
		UpdateVertexData(myDebugLines[i]);
		UINT pointAmountToRender = max(static_cast<UINT>(myDebugLines[i].GetAmountOfPoints()), 0);
		context->Draw(pointAmountToRender, 0);
	}
	
	for (CLines& line : myLuaLines)
	{
		//std::lock_guard<mutex> safe(myLineMutex);
		UpdateVertexData(line);
		UINT pointAmountToRender = max(static_cast<UINT>(line.GetAmountOfPoints()), 0);
		context->Draw(pointAmountToRender, 0);
	}
	
}

void CDebugDrawer::UpdateViewPositionBuffer(CCameraInstance * aCameraInstance)
{
	myViewData.myToView = aCameraInstance->GetPosition();
	ID3D11DeviceContext* context = myEngine->GetDXFrameWork()->GetContext();

	D3D11_MAPPED_SUBRESOURCE data;
	HRESULT result;

	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = context->Map(myViewPositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "Camera failed to map buffers in UpdateViewPositionBuffers");
	}

	memcpy(data.pData, &myViewData, sizeof(myViewData));
	context->Unmap(myViewPositionBuffer, 0);
}

void CDebugDrawer::UpdateInstanceBuffer(SInstanceBufferData dataToSet)
{
	ID3D11DeviceContext* context = myEngine->GetDXFrameWork()->GetContext();

	D3D11_MAPPED_SUBRESOURCE data;
	HRESULT result;

	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = context->Map(myViewPositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "Camera failed to map buffers in UpdateViewPositionBuffers");
	}

	memcpy(data.pData, &dataToSet, sizeof(dataToSet));
	context->Unmap(myViewPositionBuffer, 0);
}

void CDebugDrawer::CreateViewPositionBuffer()
{
	ID3D11Device* device = myEngine->GetDXFrameWork()->GetDevice();
	ID3D11Buffer* viewBuffer;
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.ByteWidth = sizeof(SViewPositionBufferData);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = device->CreateBuffer(&bufferDescription, nullptr, &viewBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "DebugDrawer failed to create ViewPositionBuffer");
		if (result == E_INVALIDARG)
		{
			DL_ERROR(eDebugLogType::Engine, "%s", "invalidArgument !?");
		}
	}
	myViewPositionBuffer = viewBuffer;
}

void CDebugDrawer::CreateInstanceBuffer()
{
	HRESULT result;
	ID3D11Device* device = myEngine->GetDXFrameWork()->GetDevice();

	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.ByteWidth = sizeof(SInstanceBufferData);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = device->CreateBuffer(&bufferDescription, nullptr, &myInstanceBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "DebugDrawer failed to create instancebuffer");
	}
}

void CDebugDrawer::CreateLineVertexBuffer()
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.ByteWidth = static_cast<UINT>(MAXAMOUNTOFPOINTSONALINE + 1) * sizeof(SLinePoint);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = myEngine->GetDXFrameWork()->GetDevice()->CreateBuffer(&bufferDescription, nullptr, &myLineVertexBuffer);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "failed to initialize LineVertexBuffer");
	}
	myLineStride = sizeof(SLinePoint);
}

void CDebugDrawer::CreateShaders()
{
	HRESULT result;
	ID3D11Device* device = myEngine->GetDXFrameWork()->GetDevice();
	
	//std::ifstream vsFile;
	//vsFile.open("../Assets/Shaders/VS_Line.hlsl", std::ios::binary);
	//std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	//vsFile.close();
	ID3D10Blob* VSBlob = nullptr;
	ID3D10Blob* PSBlob = nullptr;

	myShaderCompiler.CompileShader("../Assets/Shaders/VS_Line.hlsl", "main", "vs_5_0", VSBlob);
	myShaderCompiler.CompileShader("../Assets/Shaders/PS_Line.hlsl", "main", "ps_5_0", PSBlob);

	result = device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &mySingleLineVertexShader);
	result = device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &mySingleLinePixelShader);

	//std::ifstream psFile;
	//psFile.open("../Assets/Shaders/PS_Line.hlsl", std::ios::binary);
	//std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	//psFile.close();


	CreateSingleLineInputLayout(VSBlob);

	VSBlob->Release();
	VSBlob = 0;
	PSBlob->Release();
	PSBlob = 0;
}

void CDebugDrawer::Update()
{
	myCPUUseReader.Update();
	PROCESS_MEMORY_COUNTERS memCounter;
	GetProcessMemoryInfo(GetCurrentProcess(),	&memCounter, sizeof(memCounter));
	const size_t memUsedMb = memCounter.WorkingSetSize / 1024 / 1024;


	const float myCurrentDeltaTime = Timer::GetInstance().GetDeltaTime();;
	const float fps = (myLastDeltaTime + myCurrentDeltaTime) / 2.f;
	

	if (myDebugInformationTimer <= 0.f)
	{
		myDebugString = "FrameTime (ms): " + std::to_string(fps * 1000) + "\n";
		myDebugString += "FPS: " + std::to_string(static_cast<int>((1 / fps) / 1)) + "\n";
		myDebugString += "CPU Usage: " + std::to_string(myCPUUseReader.GetCpuUsage()) + "%\n";
		myDebugString += "Memory Usage: " + std::to_string(memUsedMb) + "mb";
		myDebugString += myTextToAddToPrint;
		myDebugInformationTimer = 0.2f;
	}
	myTextToAddToPrint = "";
	myDebugInformationTimer -= fps;
	myLastDeltaTime = myCurrentDeltaTime;
}

void CDebugDrawer::CreateSingleLineInputLayout(ID3D10Blob* aBlob)
{
	ID3D11Device* device = myEngine->GetDXFrameWork()->GetDevice();
	HRESULT result = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA },
		{ "WIDTH", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA },
	};
	result = device->CreateInputLayout(layout, 3, aBlob->GetBufferPointer(), aBlob->GetBufferSize(), &mySingleLineInputLayout);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "Create Line InputLayout failed to load");
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Engine, "%s", "Create Line InputLayout load succeded");
	}
}

void CDebugDrawer::UpdateVertexData(CLines & aLine)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE data;
	SLinePoint* LinesToMemcpy = aLine.GetPoints().GetPureData();
	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = myEngine->GetDXFrameWork()->GetContext()->Map(myLineVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "LineRenderer failed to map buffers in UpdateVertexData");
	}
	memcpy(data.pData, LinesToMemcpy, (sizeof(SLinePoint) * (aLine.GetPoints().Size())));
	myEngine->GetDXFrameWork()->GetContext()->Unmap(myLineVertexBuffer, 0);
}

#endif

