#pragma once
class CCameraInstance;
class CDXFramework;
struct ID3D11Buffer;

struct SCameraBufferData
{
	Matrix44 myCameraOrientation;
	Matrix44 myToCamera;
	Matrix44 myProjection;

};

class CCamera
{
public:
	CCamera();
	~CCamera();


	bool Init(CDXFramework* aDXFramework, Matrix44 aProjectionMatrix);

	void UpdateBuffers(CDXFramework* aDirectXFramework, CCameraInstance* aCameraInstance);
	void Transform(const Matrix44 & aTransformation);
	ID3D11Buffer* GetBuffer();

	const SCameraBufferData& GetData();

private:

	SCameraBufferData myCameraData;
	ID3D11Buffer* myCameraBuffer;
};

