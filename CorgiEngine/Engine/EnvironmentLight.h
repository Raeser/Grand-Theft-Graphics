#pragma once

struct SDirectionLightBufferData
{
	Vector4f myDirection;
	Vector4f myColor;
};

struct SEnvironmentLightData 
{
	ID3D11ShaderResourceView* myCubeMap;
	ID3D11Buffer* myDirectionalLightBuffer;
};

class CEnvironmentLight
{
public:
	CEnvironmentLight();
	~CEnvironmentLight();

	bool Init(const SEnvironmentLightData& aLightData);
	void Release();
	const SEnvironmentLightData& GetEnvironmentLightBufferData() const;

	SEnvironmentLightData& GetEnvironmentLightBufferData();
	bool UpdateBuffer(const SDirectionLightBufferData& aData, ID3D11DeviceContext* aContext);



private:
	SEnvironmentLightData myData;
};

