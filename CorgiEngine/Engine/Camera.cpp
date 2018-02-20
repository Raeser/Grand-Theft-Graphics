#include "stdafx.h"
#include "CameraInstance.h"


CCamera::CCamera()
{
}


CCamera::~CCamera()
{
	myCameraBuffer->Release();
}

bool CCamera::Init(CDXFramework * aDXFramework, Matrix44 aProjectionMatrix)
{
	myCameraData.myProjection = aProjectionMatrix;

	HRESULT result;

	ID3D11Device* device = aDXFramework->GetDevice();

	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.ByteWidth = sizeof(SCameraBufferData);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	result = device->CreateBuffer(&bufferDescription, nullptr, &myCameraBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "Camera failed to create buffer");
		return false;
	}
	
	return true;
}

void CCamera::UpdateBuffers(CDXFramework * aDirectXFramework, CCameraInstance * aCameraInstance)
{
	myCameraData.myCameraOrientation = aCameraInstance->myOrientation;
	myCameraData.myToCamera = myCameraData.myCameraOrientation.GetFastInverse();

	ID3D11DeviceContext* context = aDirectXFramework->GetContext();
	D3D11_MAPPED_SUBRESOURCE data;
	HRESULT result;

	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = context->Map(myCameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Engine, "%s", "Camera failed to map buffers in UpdateBuffers");
	}

	memcpy(data.pData, &myCameraData, sizeof(myCameraData));
	context->Unmap(myCameraBuffer, 0);
}

void CCamera::Transform(const Matrix44 & aTransformation)
{
	aTransformation;
	//myCameraData.myCameraOrientation *= aTransformation;
}

ID3D11Buffer * CCamera::GetBuffer()
{
	return myCameraBuffer;
}

const SCameraBufferData& CCamera::GetData()
{
	return myCameraData;
}
