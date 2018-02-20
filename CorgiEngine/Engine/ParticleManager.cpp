#include "stdafx.h"
#include "ParticleManager.h"
#include "ParticleEmitterInstance.h"
#include "ParticleEmitterLoader.h"
#include "ParticleEmitter.h"
#include "../Utilities/Sort.h"

CParticleManager::CParticleManager(CParticleEmitterLoader & aEmittorLoader):
	 myLoader(aEmittorLoader)
{
}

CParticleManager::~CParticleManager()
{
}

bool CParticleManager::Init()
{
	myRenderData.Init(256);
	myParticleEmittorInstances.Init(256);
	return true;
}

void CParticleManager::AssignParticleEmittorInstances(const Corgi::GrowingArray<CParticleEmitterInstance*>& aEmittorInstances)
{
	myParticleEmittorInstances = aEmittorInstances;
	//myParticleEmittorInstances.RemoveAll();
	//for (int i = 0; i < aEmittorInstances.Size(); ++i)
	//{
	//	myParticleEmittorInstances.Add(aEmittorInstances[i]);
	//}
}

CParticleEmitterLoader & CParticleManager::GetParticleLoader()
{
	return myLoader;
}

void CParticleManager::Update(const Vector3f& aPositionToSortAfter)
{
	//for (CParticleEmitterInstance* instance : myParticleEmittorInstances)
	//{
	//	if (instance->GetShouldClear() || instance->GetShouldUpdate())
	//	{
	//		//instance->Update(myLoader.GetEmitter(instance->GetEmitterID()).GetParticleData());
	//	}
	//}
	SortInstances(aPositionToSortAfter);
	myRenderData = myParticleEmittorInstances;
}

Corgi::GrowingArray<CParticleEmitterInstance*>& CParticleManager::GetEmittorData()
{
	return myRenderData;
}



void CParticleManager::SortInstances(const Vector3f & aPositionToSortAfter)
{
	float distance;
	std::vector<CParticleEmitterInstance*> sortingVector;
	for (int i = 0; i < myParticleEmittorInstances.Size(); ++i)
	{
		distance = aPositionToSortAfter.Dot(myParticleEmittorInstances[i]->GetPosition());
		myParticleEmittorInstances[i]->SetDistance(distance);
		sortingVector.push_back(myParticleEmittorInstances[i]);
	}
	CommonUtilities::MergeSortPointers(sortingVector);
	
	myParticleEmittorInstances.RemoveAll();
	myParticleEmittorInstances.ReInit(static_cast<int>(sortingVector.size()));
	for (int i = 0; i < sortingVector.size(); ++i)
	{
		myParticleEmittorInstances.Add(sortingVector[i]);
	}
}

