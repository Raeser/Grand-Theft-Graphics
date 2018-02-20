#pragma once
#include "../Utilities/Matrix44.h"
class CScene;
class CAnimationInstance;
class CModelInstance
{
	friend class CScene;
	friend class CDeferredRenderer;
	friend class CForwardRenderer;
	friend class CRenderComponent;
public:
	CModelInstance();
	~CModelInstance();
	void SetModel(const long long aModelID);
	void SetVertexShaderID(const long long aVertexShaderID);
	void SetPixelShaderID(const long long aPixelShaderID);
	void SetGeometryShaderID(const long long aGeometryShaderID);
	const long long GetVertexShaderID();
	const long long GetPixelShaderID();
	const long long GetGeometryShaderID();

	bool DidAnimationFinishThisFrame();

	Matrix44 GetOrientation();
	Vector3f GetPosition();
	
	void Animate(std::string animationName, bool aShouldLoop);

	void Transform(const Matrix44& aTransformation);
	void Move(const Vector3f& aMovement);
	const long long& GetModelId() const { return myModel; }
	void SetOrientation(const Matrix44& aOrientation);
	void SetPosition(const Vector3f& aPosition);
	void SetShouldRender(bool aShouldRender);
	bool GetShouldRender();
	const CAnimationInstance* GetConstAnimationInstance() const;
	void SetIsStatic(bool isStatic);

private:

	void SetAnimationInstance(CAnimationInstance* aAnimationInstance, long long aID);
	CAnimationInstance* GetAnimationInstance() const;
	bool IsAnimationReady();

private:
	Matrix44 myOrientation;
	std::string myAnimationName;
	long long myModel;
	long long myVertexShaderID;
	long long myPixelShaderID;
	long long myGeometryShaderID;
	long long myAnimationInstanceID;
	bool myShouldRender;
	bool myIsAnimationInitialized;
	bool myAnimateThisObject;
	bool myAnimationShouldLoop;
	bool myIsStatic;
	CAnimationInstance* myAnimationInstance;
};

