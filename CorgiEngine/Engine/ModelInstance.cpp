#include "stdafx.h"
#include "ModelInstance.h"
#include "AnimationInstance.h"

CModelInstance::CModelInstance()
{
	myAnimationInstance = nullptr;
	myOrientation = Matrix44();
	myShouldRender = true;
	myGeometryShaderID = 0;
	myPixelShaderID = 0;
	myVertexShaderID = 0;
	myIsAnimationInitialized = false;
	myAnimateThisObject = false;
}


CModelInstance::~CModelInstance()
{
}

void CModelInstance::SetModel(const long long aModelID)
{
	myModel = aModelID;
}


void CModelInstance::SetVertexShaderID(const long long aVertexShaderID)
{
	myVertexShaderID = aVertexShaderID;
}

void CModelInstance::SetPixelShaderID(const long long aPixelShaderID)
{
	myPixelShaderID = aPixelShaderID;
}

void CModelInstance::SetGeometryShaderID(const long long aGeometryShaderID)
{
	myGeometryShaderID = aGeometryShaderID;
}

const long long CModelInstance::GetVertexShaderID()
{
	return myVertexShaderID;
}

const long long CModelInstance::GetPixelShaderID()
{
	return myPixelShaderID;
}

const long long CModelInstance::GetGeometryShaderID()
{
	return myGeometryShaderID;
}

bool CModelInstance::DidAnimationFinishThisFrame()
{
	if (myAnimationInstance)
	{
		return myAnimationInstance->AnimationFinishedThisFrame();
	}
	else
	{
		return false;
	}
}

Matrix44 CModelInstance::GetOrientation()
{
	return myOrientation;
}

Vector3f CModelInstance::GetPosition()
{
	return Vector3f(myOrientation.myMatrix[12], myOrientation.myMatrix[13], myOrientation.myMatrix[14]);
}

void CModelInstance::Animate(std::string animationName, bool aShouldLoop)
{
	myAnimateThisObject = true;
	myAnimationShouldLoop = aShouldLoop;
	myAnimationName = animationName;
	if (myAnimationInstance)
	{
		myAnimationInstance->Reset();

	}
}

void CModelInstance::SetAnimationInstance(CAnimationInstance * aAnimationInstance, long long aID)
{
	myAnimationInstance = aAnimationInstance;
	if (myAnimationInstance != nullptr)
	{
		myAnimationInstanceID = aID;
		myIsAnimationInitialized = true;
		myAnimateThisObject = false;
		myAnimationInstance->Play();
		myAnimationInstance->SetShouldLoop(myAnimationShouldLoop);
	}
}

CAnimationInstance * CModelInstance::GetAnimationInstance() const
{
	return myAnimationInstance;
}

bool CModelInstance::IsAnimationReady()
{
	return myIsAnimationInitialized;
}

void CModelInstance::Transform(const Matrix44 & aTransformation)
{
	myOrientation *= aTransformation;
}

void CModelInstance::Move(const Vector3f & aMovement)
{
	myOrientation.myMatrix[12] += aMovement.x;
	myOrientation.myMatrix[13] += aMovement.y;
	myOrientation.myMatrix[14] += aMovement.z;
}

void CModelInstance::SetOrientation(const Matrix44 & aOrientation)
{
	myOrientation = aOrientation;
}

void CModelInstance::SetPosition(const Vector3f & aPosition)
{
	myOrientation.myMatrix[12] = aPosition.x;
	myOrientation.myMatrix[13] = aPosition.y;
	myOrientation.myMatrix[14] = aPosition.z;

}

void CModelInstance::SetShouldRender(bool aShouldRender)
{
	myShouldRender = aShouldRender;
}

bool CModelInstance::GetShouldRender()
{
	return myShouldRender;
}

const CAnimationInstance * CModelInstance::GetConstAnimationInstance() const
{
	return myAnimationInstance;
}

void CModelInstance::SetIsStatic(bool isStatic)
{
	myIsStatic = isStatic;
}
