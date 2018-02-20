#ifndef _RETAIL ////We dont use debugdrawer in retail environment

#pragma once
#include <mutex>
#include "TextManager.h"
#include "CpuUsage.h"
#include "..\Utilities\Vector.h"
#include "..\Engine\ShaderCompiler.h"

class CEngine;
class CModelInstance;
class CLines;
struct lua_State;
class CCameraInstance;


class CDebugDrawer
{
public:
	~CDebugDrawer();

	static CDebugDrawer& GetInstance()
	{
		static CDebugDrawer me;
		return me;
	}
	void Init(CEngine* aEngine);
	void CreateShaders();
	void Update();
	void EndFrame();
	CModelInstance* CreateCube(const Vector3f& aPos); //USE ONLY IN INIT! this will give you a cube that you can manage yourself, the pointer is stored like all other models inside scene so do not delete it
	CModelInstance* CreateSphere(const Vector3f& aPos); //USE ONLY IN INIT! this will give you a sphere that you can manage yourself, the pointer is stored like all other models inside scene so do not delete it
	void DrawCube(const Vector3f& aPos); //USE IN UPDATE! this will spawn a cube at the set position, but you need to call this function on every update! CreateCube is better if you're in the market for something faster!
	void DrawSphere(const Vector3f& aPos); //USE IN UPDATE! this will spawn a sphere at the set position, but you need to call this function on every update! CreateSphere is better if you're in the market for something faster!
	void DrawLine(const Vector4f& aFromPos, const Vector4f& aToPos);
	void DrawLine(Vector4f* aPointArray, int AmountOfPoints);
	void DrawArrow(const Vector4f& aFromPos, const Vector4f& aToPos, const Vector3f aLineColor = {1,0,0}, const Vector3f aArrowColor = { 1,0,0 }, int aQuality = 1);
	const int& GetActiveLinesSize() const;
	void DrawDebugText(std::string aText); //This will print in top left corner, and print under the previously rendered text. This needs to be called every frame
	void RenderLines(CCameraInstance* aCameraInstance);
	//void DrawText(const char* aText, Vector2f aScreenPos); //This will print on a set ScreenPosition
	//void DrawText(const char* aText, Vector3f aWorldPos); //This will maybe be implemented


	int DrawArrowFromLua(lua_State * aState);

	int DrawCubeFromLua(lua_State * aState);

	int DrawLineFromLua(lua_State * aState);

	int PrintTextFromLua(lua_State * aState);
private:
	struct SInstanceBufferData
	{
		Vector4f myFromPosition;
		Vector4f myToPosition;
	};

	struct SViewPositionBufferData
	{
		Vector3f myToView = Vector3f();
		float myTrash;
	};

	void UpdateViewPositionBuffer(CCameraInstance* aCameraInstance);
	void UpdateInstanceBuffer(SInstanceBufferData dataToSet);
	void CreateViewPositionBuffer();
	void CreateInstanceBuffer();
	void CreateLineVertexBuffer();
	void CreateSingleLineInputLayout(ID3D10Blob* aBlob);
	void UpdateVertexData(CLines& aLine);

	void SetUpArrow(CLines& aLineconst, const Vector4f & aFromPos, const Vector4f & aToPos, const Vector3f aLineColor = { 1,0,0 }, const Vector3f aArrowColor = { 1,0,0 }, int aQuality = 1);


	CDebugDrawer();
	Corgi::CMemoryPool<CModelInstance*> myCubes;
	Corgi::CMemoryPool<CModelInstance*> mySpheres;
	Corgi::CMemoryPool<CLines> myDebugLines;
	Corgi::GrowingArray<CLines> myLuaLines;


	SViewPositionBufferData myViewData;
	long long myCubeID;
	long long mySphereID;

	ID3D11InputLayout* mySingleLineInputLayout;
	ID3D11VertexShader* mySingleLineVertexShader;
	ID3D11PixelShader* mySingleLinePixelShader;
	ID3D11Buffer* myViewPositionBuffer;
	ID3D11Buffer* myInstanceBuffer;
	ID3D11Buffer* myLineVertexBuffer;
	CEngine* myEngine;
	TextID myDebugText;
	std::string myDebugString;
	std::string myTextToAddToPrint;
	std::string myDebugTextFromLua;
	UINT myLineStride; 

	float myDebugInformationTimer;
	float myLastDeltaTime;

	int myAmountOfActiveLines;
	int myAmountOfActiveSpheres;
	int myAmountOfActiveCubes;

	std::mutex myLineMutex;
	CCpuUsage myCPUUseReader;

	CShaderCompiler myShaderCompiler;
};

#endif