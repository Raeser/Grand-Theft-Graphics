#include "stdafx.h"
#include "EnvironmentLight.h"


CEnvironmentLight::CEnvironmentLight()
{
}


CEnvironmentLight::~CEnvironmentLight()
{

}

bool CEnvironmentLight::Init( const SEnvironmentLightData & aLightData)
{
	myData = aLightData;
	return true;
}

void CEnvironmentLight::Release()
{
	myData.myDirectionalLightBuffer->Release();
	myData.myCubeMap->Release();
}

const SEnvironmentLightData& CEnvironmentLight::GetEnvironmentLightBufferData() const
{
	return myData;
}


SEnvironmentLightData & CEnvironmentLight::GetEnvironmentLightBufferData()
{
	return myData;
}

bool CEnvironmentLight::UpdateBuffer(const SDirectionLightBufferData & aData, ID3D11DeviceContext* aContext)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE data;

	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = aContext->Map(myData.myDirectionalLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "EnvironmentLight failed to map buffers in UpdateBuffer");
		return false;
	}

	memcpy(data.pData, &aData, sizeof(aData));
	aContext->Unmap(myData.myDirectionalLightBuffer, 0);

	return true;
}
