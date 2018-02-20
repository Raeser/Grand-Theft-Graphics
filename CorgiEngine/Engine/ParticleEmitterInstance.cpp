#include "stdafx.h"
#include "ParticleEmitterInstance.h"
#include "ParticleDataWrapper.h"
#include "../Utilities/Sort.h"
#include "ParticleEmitter.h"

CParticleEmitterInstance::CParticleEmitterInstance()
{
	myParticles.Init(4, false);
	myGAReinitedWithMax = false;
	myCoolDown = 0;
}


CParticleEmitterInstance::~CParticleEmitterInstance()
{
}

void CParticleEmitterInstance::Init(long long aParentID, int aMaxAmountOfParticles)
{
	myParticleEmitter = aParentID;
	myParticles.ReInit(aMaxAmountOfParticles);
	myGAReinitedWithMax = true;
	SParticle newParticle;
	newParticle.currentLifetime = 0;
	newParticle.position = { 0,0,0,0 };
	newParticle.direction = { 0,0,0 };
	newParticle.currentLifetime = 0;
	newParticle.isAlive = false;
	newParticle.timesEmitted = 0;
	for (int i = 0; i < aMaxAmountOfParticles; ++i)
	{
		myParticles.Add(newParticle);
	}
	myCooldown = 0.f;
}

void CParticleEmitterInstance::Transform(const Matrix44 & aTransformation)
{
	myOrientation = aTransformation * myOrientation;
}

void CParticleEmitterInstance::Move(const Vector3f & aMovement)
{
	myOrientation.myMatrix[12] += aMovement.x;
	myOrientation.myMatrix[13] += aMovement.y;
	myOrientation.myMatrix[14] += aMovement.z;
}

void CParticleEmitterInstance::SetEmitterID(long long aID)
{
	myGAReinitedWithMax = false;
	myParticleEmitter = aID;
}

void CParticleEmitterInstance::SetOrientation(const Matrix44 & aOrientation)
{
	myOrientation = aOrientation;
}

Matrix44 & CParticleEmitterInstance::GetOrientation()
{
	return myOrientation;
}


bool CParticleEmitterInstance::GetIsActive()
{
	return myIsActive;
}

bool CParticleEmitterInstance::GetShouldRender()
{
	return myShouldRender;
}

bool CParticleEmitterInstance::GetShouldUpdate()
{
	return myShouldUpdateParticles;
}

bool CParticleEmitterInstance::GetShouldClear()
{
	return myShouldClearParticles;
}

const Vector3f CParticleEmitterInstance::GetPosition()
{
	return Vector3f(myOrientation.myMatrix[12], myOrientation.myMatrix[13], myOrientation.myMatrix[14]);
}

float CParticleEmitterInstance::GetDistance()
{
	return myDistanceToCamera;
}

bool CParticleEmitterInstance::GetIsCorrectlyInitialized()
{
	return myGAReinitedWithMax;
}

void CParticleEmitterInstance::Stop()
{
	myShouldSpawnParticles = false;
	myIsActive = false;
}

void CParticleEmitterInstance::Start()
{
	myShouldSpawnParticles = true;
	myShouldRender = true;
	myShouldUpdateParticles = true;
}

float & CParticleEmitterInstance::GetCoolDown()
{
	return myCoolDown;
}

CParticleEmitterInstance::Spawn & CParticleEmitterInstance::GetSpawnData()
{
	return mySpawnData;
}

Corgi::GrowingArray<SParticle, long long>& CParticleEmitterInstance::GetParticles()
{
	return myParticles;
}

long long CParticleEmitterInstance::GetAmountOfParticles()
{
	return myParticles.Size();
}

void CParticleEmitterInstance::SetIsActive(bool aIsActive)
{
	myIsActive = aIsActive;
	myShouldRender = aIsActive;
	myShouldUpdateParticles = aIsActive;
}

void CParticleEmitterInstance::SetShouldRender(bool aShouldRender)
{
	myShouldRender = aShouldRender;
}

void CParticleEmitterInstance::SetShouldUpdateParticles(bool aShouldUpdate)
{
	myShouldUpdateParticles = aShouldUpdate;
}

void CParticleEmitterInstance::SetClearState(bool aShouldClear)
{
	myShouldClearParticles = aShouldClear;
}

void CParticleEmitterInstance::SetDistance(float aDistance)
{
	myDistanceToCamera = aDistance;
}

void CParticleEmitterInstance::SortParticles(const Vector3f & aPositionToSortAfter)
{
	float distance = 0;
	std::vector<SParticle> sortingVector;
	for (int i = 0; i < myParticles.Size(); ++i)
	{
		distance = (aPositionToSortAfter - myParticles[i].position.V3()).Length2();
	//	myParticles[i].SetDistance(distance);
		sortingVector.push_back(myParticles[i]);
	}
//	CommonUtilities::MergeSort(sortingVector);

	myParticles.RemoveAll();
	for (int i = 0; i < sortingVector.size(); ++i)
	{
		myParticles.Add(sortingVector[i]);
	}
}



bool CParticleEmitterInstance::operator<(const CParticleEmitterInstance& aInstance)
{
	return myDistanceToCamera < aInstance.myDistanceToCamera;
}
