#pragma once

enum EmitterType
{
	eEmitterType_Point = 0,
	eEmitterType_Rectangle
};


struct SEmitter
{
	EmitterType emittertype;
	int maxparticles;
	Vector3f velocity;
	float emissiondelay;
	float lifetime;
	float gravity;
	Vector4f color0;
	Vector4f color1;
	Vector4f color2;
	float startSizeX;
	float startSizeY;
	float endSizeX;
	float endSizeY;
	float rectSizeX;
	float rectSizeZ;
	int textureType;
	int textureColumns;
	int textureRows;
	int looping;
};



//struct SParticleEmitterData
//{
//	float myStartScale;
//	float myEndScale;
//
//	float myMinTimeBetweenParticleSpawns;
//	float myMaxTimeBetweenParticleSpawns;
//
//	float myMinStartSpeed;
//	float myMaxStartSpeed;
//
//	float myMinAngle;
//	float myMaxAngle;
//
//	float myMinLifeTime;
//	float myMaxLifeTime;
//
//	Corgi::Vector3<float> myStartColor;
//	Corgi::Vector3<float> myEndColor;
//
//	Corgi::Vector3<float> myAcceleration;
//
//	//EBlendState myBlendState;
//};