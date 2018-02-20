#pragma once

struct SParticle;
class CParticleEmitterInstance
{
private:
	struct Spawn
	{
		float last;
		float next;

	};
public:
	CParticleEmitterInstance();
	~CParticleEmitterInstance();
	bool operator<(const CParticleEmitterInstance& aInstance);

	void Init(long long aParentID, int aMaxAmountOfParticles);
	//void Update(const SParticleEmitterData& aData);

	void Transform(const Matrix44& aTransformation);
	void Move(const Vector3f& aMovement);
	void SetEmitterID(long long aID);
	const long long& GetEmitterID() const { return myParticleEmitter; }
	void SetOrientation(const Matrix44& aOrientation);
	Matrix44& GetOrientation();
	Spawn& GetSpawnData();
	Corgi::GrowingArray<SParticle, long long>& GetParticles();
	long long GetAmountOfParticles();
	void SetIsActive(bool aIsActive);
	void SetShouldRender(bool aShouldRender);
	void SetShouldUpdateParticles(bool aShouldUpdate);
	void SetClearState(bool aShouldClear);
	void SetDistance(float aDistance);
	bool GetIsActive();
	bool GetShouldRender();
	bool GetShouldUpdate(); 
	bool GetShouldClear();
	const Vector3f GetPosition();
	float GetDistance();
	bool GetIsCorrectlyInitialized();

	float& GetCooldown() { return myCooldown; }
	
	void Stop();
	void Start();

	float& GetCoolDown();
	//void SpawnNewParticles(const SParticleEmitterData& aData);

	void SortParticles(const Vector3f & aPositionToSortAfter);
private:

	long long myParticleEmitter;
	float myCoolDown;
	Corgi::GrowingArray<SParticle, long long> myParticles;
	Matrix44 myOrientation;
	bool myIsActive;
	bool myShouldUpdateParticles;
	bool myShouldSpawnParticles;
	bool myShouldRender;
	bool myShouldClearParticles;
	bool myGAReinitedWithMax;
	Spawn mySpawnData;
	float myDistanceToCamera;

	float myCooldown;
};
