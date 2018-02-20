#pragma once

struct SPlayerData
{
	Corgi::Vector3<float> position;
	Corgi::Vector3<float> rotation;
	int health;
	float playerSpeed;
	float clickIndicatorSpeed;
	float clickIndicatorStartHeight;
	float clickIndicatorEndHeight;
	float clickIndicatorLifeTime;
};

struct STikiData
{
	float tikiSpeed;
	float throwSpeed;
	float maxThrowHeight;
	float maxThrowLength;
	float jumpSpeed;
	float minDistianceToPlayer;
	float restlessMaxTimer;
};
struct SSettingsData
{
	float AimAssistDistiance;
};

struct SEditorSettings
{
	Corgi::Vector3<float> cameraDistianceVectorToPlayer;
	float cameraScrollZoomSpeed;
	float cameraMouseZoomSpeed;
	float cameraMouseZoomMultiplier;
	float cameraMouseRotationSpeed;
	float cameraMouseRotationMultiplier;
	float cameraSpeed;
	float cameraMouseDragSpeed;
	float cameraAngleInDegrees;
};

struct SConfigData
{
	Corgi::GrowingArray<std::string> levelsToLoad;
};

struct SMiscData
{
	float randParticleEndDegreeFrom;
	float randParticleEndDegreeTo;
};

