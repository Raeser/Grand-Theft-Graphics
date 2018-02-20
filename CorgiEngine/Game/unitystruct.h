#pragma once
#include "stdafx.h"

enum class EExportType
{
	EModel,
	Count
};

enum class EExportTag
{
	Asteroid,
	Enemy,
	Structure,
	Object,
	MovableObject,
	CheckPoint,
	Pickup,
	Trigger,
	PointLight,
	SpotLight,
	Wave,
	ParticleEmittor,
	AudioSpline,
	Data,
	Collider,
	count,
};

enum class EPickupType
{
	Health,
	Boost,
	Sheild,
	Count
};

struct SCollider
{
	Corgi::Vector3<float> localPosition;
	float radius;
};

struct SBoxCollider
{
	Corgi::Vector3<float> size;
};

struct SSphereCollider
{
	float radius;
};

struct SUnityExporterEnemy
{
	Corgi::Vector3<float> position;
	Corgi::Vector3<float> rotation;
	SCollider colliderHit;
	SCollider colliderAimAssist;
	SCollider colliderTrigger;
	Corgi::GrowingArray<std::string> fbxFilePaths;
};

struct SUnityExporterObject
{
	Corgi::Vector3<float> position;
	Corgi::Vector3<float> rotation;
	Corgi::GrowingArray<std::string> fbxFilePaths;
	std::string objectID;
};

struct SUNityExporterMovableObject
{
	Corgi::Vector3<float> position;
	Corgi::Vector3<float> rotation;
	Corgi::GrowingArray<std::string> fbxFilePaths;
	std::string objectID;
	float speed;
};

struct SUnityExporterWave
{
	Corgi::GrowingArray<SUnityExporterEnemy> enemies;
	float timer;
	int waveIndex;
};


struct SUnityExporterAsteroid
{
	Corgi::Vector3<float> position;
	Corgi::Vector3<float> rotation;
	SCollider colliderHit;
	Corgi::GrowingArray<std::string> fbxFilePaths;
};
struct SUnityExporterStructure
{
	Corgi::Vector3<float> position;
	Corgi::Vector3<float> rotation;
	Corgi::GrowingArray<SCollider> colliderHitArray;
	Corgi::GrowingArray<std::string> fbxFilePaths;
};
struct SUnityExportCheckPoint
{
	Corgi::Vector3<float> position;
	SCollider colliderCheckPoint;
	int index;
};
struct SUnityExporterPickup
{
	Corgi::Vector3<float> position;
	SCollider colliderHit;
	int index;
	float amount;
	EPickupType aPickupType;
	Corgi::GrowingArray<std::string> fbxFilePaths;
};
struct SUnityExporterTrigger
{
	Corgi::Vector3<float> position;
	Corgi::Vector3<float> rotation;
	SSphereCollider sphereCollider;
	SBoxCollider boxCollider;
	bool isBoxCollider;
	std::string luaFunctionToCall;
	std::string linkedObjectID;
};

struct SUnityExporterParticleEmittor
{
	Corgi::Vector3<float> position;
	Corgi::Vector3<float> rotation;
	std::string particleEmittorName;
};

struct SUnityExporterPointLight
{
	Corgi::Vector3<float> position;
	Corgi::Vector3<float> color;
	float range;
	float intensity;
};

struct SUnityExporterSpotLight
{
	Corgi::Vector3<float> position;
	Corgi::Vector3<float> rotation;
	Corgi::Vector3<float> color;
	float range;
	float intensity;
	float spotLightAngle;
};

struct SUnityExporterAudioSplines
{
	std::string audioString;
	Corgi::GrowingArray<Corgi::Vector3<float>> nodePositions;
};

struct SUnityExporterData
{
	std::string dataString;
	std::string dataTypeString;
	Corgi::Vector3<float> position;
};

struct SUnityExporterColliderNavMesh
{
	std::string IDString;
	Corgi::Vector3<float> position;
	Corgi::Vector3<float> rotation;
	Corgi::Vector3<float> size;
};

struct SUnityExporterColliderWall
{
	std::string IDString;
	Corgi::Vector3<float> position;
	Corgi::Vector3<float> rotation;
	Corgi::Vector3<float> size;
};
