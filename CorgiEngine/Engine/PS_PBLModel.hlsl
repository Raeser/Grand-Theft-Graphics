#include "CS_PBLModel.hlsli"

PixelOutput PixelShader_CalculatePBR(VertexToPixel input, LightCalculationData lightInput);



PixelOutput PixelShader_DirectDiffuse(VertexToPixel input, LightCalculationData lightInput);
PixelOutput PixelShader_Lambert(VertexToPixel input, LightCalculationData lightInput);
PixelOutput PixelShader_Fresnel(VertexToPixel input, LightCalculationData lightInput);
PixelOutput PixelShader_Distribution(VertexToPixel input, LightCalculationData lightInput);
PixelOutput PixelShader_DirectSpecularity(VertexToPixel input, LightCalculationData lightInput);
PixelOutput PixelShader_Visibility(VertexToPixel input, LightCalculationData lightInput);



PixelOutput PixelShader_Metalnessalbedo(VertexToPixel input);
PixelOutput PixelShader_Metalness(VertexToPixel input);
PixelOutput PixelShader_Albedo(VertexToPixel input);
PixelOutput PixelShader_Substance(VertexToPixel input);
PixelOutput PixelShader_Emissive(VertexToPixel input);
PixelOutput PixelShader_Roughness(VertexToPixel input);
PixelOutput PixelShader_ObjectNormal(VertexToPixel input);
PixelOutput PixelShader_AmbientDiffuse(VertexToPixel input);
PixelOutput PixelShader_AmbientOcclusion(VertexToPixel input);
PixelOutput PixelShader_ReflectionFresnel(VertexToPixel input);
PixelOutput PixelShader_AmbientSpecularity(VertexToPixel input);

PixelOutput PixelShader_CalculatePointLights(VertexToPixel input);
PixelOutput PixelShader_CalculateSpotLights(VertexToPixel input);


PixelOutput main(VertexToPixel input)
{
	LightCalculationData EnvironmentLightData;
	EnvironmentLightData.LightColor = DirectionalLightColor;
	EnvironmentLightData.toLight = DirectionalLightDirection.xyz * -1.0f;

	PixelOutput returnMe;
	returnMe.myColor = PixelShader_CalculatePBR(input, EnvironmentLightData).myColor;
	
	returnMe.myColor += PixelShader_CalculatePointLights(input).myColor;
	returnMe.myColor += PixelShader_CalculateSpotLights(input).myColor;
	
	returnMe.myColor.a = 1.f;
	return returnMe;
}

PixelOutput PixelShader_CalculateSpotLights(VertexToPixel input)
{
	LightCalculationData spotLightData;
	PixelOutput returnMe;
	returnMe.myColor = float4(0, 0, 0, 1);
	for (unsigned int index = 0; index < numberOfUsedSpotLights; index++)
	{
		float3 toLight = spotLights[index].myPosition.xyz - input.myWorldPosition.xyz;
		float toLightDistance = toLight.x * toLight.x + toLight.y * toLight.y + toLight.z * toLight.z;
		toLight = normalize(toLight);
		float lightRange2 = spotLights[index].myRange * spotLights[index].myRange;

		float lambertattenuation = saturate(dot(PixelShader_ObjectNormal(input).myColor.xyz, toLight));
		float linearattenuation = toLightDistance / lightRange2;
		linearattenuation = 1.0f - linearattenuation;
		linearattenuation = saturate(linearattenuation);
		float physicalattenuation = 1.0f / (toLightDistance * 0.1f);

		float halfangle = spotLights[index].myAngle / 2.f;
		float cutPoint = 1.f - (halfangle / 90.f);

		float3 fromLight = normalize(-toLight);
		float3 dir = normalize(spotLights[index].myDirection);
		float x = dot(fromLight, dir);

		float angleattenuation = (x - cutPoint) * (1.f / (1.f - cutPoint));

		float attenuation = angleattenuation * lambertattenuation * linearattenuation * physicalattenuation;


		spotLightData.LightColor.xyz = spotLights[index].myColor.xyz;
		spotLightData.LightColor.w = 1;
		spotLightData.toLight = toLight;

		returnMe.myColor.rgb += PixelShader_Albedo(input).myColor.xyz * attenuation * spotLights[index].myColor.xyz * PixelShader_CalculatePBR(input, spotLightData).myColor.xyz;
	}
	return returnMe;
}


PixelOutput PixelShader_CalculatePointLights(VertexToPixel input)
{
	LightCalculationData PointlightData;
	PixelOutput returnMe;
	returnMe.myColor = float4(0, 0, 0, 0);
	for (unsigned int index = 0; index < numberOfUsedPointLights; index++)
	{
		
		float3 toLight = pointLights[index].myPosition.xyz - input.myWorldPosition.xyz;
		float toLightDistance = toLight.x * toLight.x + toLight.y * toLight.y + toLight.z * toLight.z;
		toLight = normalize(toLight);
		float lightRange2 = pointLights[index].myRange * pointLights[index].myRange;
		float lambertattenuation = saturate(dot(PixelShader_ObjectNormal(input).myColor.xyz, toLight));
		float linearattenuation = toLightDistance / lightRange2;

		linearattenuation = 1.0f - linearattenuation;
		linearattenuation = saturate(linearattenuation);
		float physicalattenuation = 1.0f / (toLightDistance * 0.1f);
		float attenuation = lambertattenuation * linearattenuation *physicalattenuation;

		PointlightData.LightColor.xyz = pointLights[index].myColor.xyz;
		PointlightData.LightColor.w = 1;
		PointlightData.toLight = toLight;

		returnMe.myColor.rgb += PixelShader_Albedo(input).myColor.xyz * attenuation * pointLights[index].myColor.xyz * PixelShader_CalculatePBR(input, PointlightData).myColor.xyz;
	}
	return returnMe;
}

PixelOutput PixelShader_CalculatePBR(VertexToPixel input, LightCalculationData lightInput)
{



	float3 ambientDiffuse = PixelShader_AmbientDiffuse(input).myColor.rgb;
	float3 ambientSpecularity = PixelShader_AmbientSpecularity(input).myColor.rgb;
	float3 directDiffuse = PixelShader_DirectDiffuse(input, lightInput).myColor.rgb;
	float3 directSpecularity = PixelShader_DirectSpecularity(input, lightInput).myColor.rgb;
	float3 emissive = PixelShader_Emissive(input).myColor.rgb;

	PixelOutput returnMe;
	returnMe.myColor.rgb = emissive + ambientDiffuse + directDiffuse + ambientSpecularity + directSpecularity;
	returnMe.myColor.a = 1.0f;

	return returnMe;
}

PixelOutput PixelShader_DirectDiffuse(VertexToPixel input, LightCalculationData lightInput)
{
	float3 lightColor = lightInput.LightColor.xyz;

	float3 metalnessalbedo = PixelShader_Metalnessalbedo(input).myColor.rgb;
	
	float3 lambert = PixelShader_Lambert(input, lightInput).myColor.xxx;
	float3 fresnel = PixelShader_Fresnel(input, lightInput).myColor.xyz;

	PixelOutput output;
	output.myColor.rgb = metalnessalbedo * lightColor * lambert * (float3(1.0f, 1.0f, 1.0f) - fresnel);
	output.myColor.a = 1.0f;
	return output;
}

PixelOutput PixelShader_Metalnessalbedo(VertexToPixel input)
{
	float4 albedo = PixelShader_Albedo(input).myColor.rgba;
	float3 metalness = PixelShader_Metalness(input).myColor.xyz;
	float3 metalnessalbedo = albedo.rgb - (albedo.rgb * metalness);
	
	//if (albedo.a <= 0.0f)
	//{
	//	discard;
	//}

	PixelOutput output;
	output.myColor.rgb = metalnessalbedo.xyz;
	output.myColor.a = albedo.a;
	return output;
}

PixelOutput PixelShader_Metalness(VertexToPixel input)
{
	PixelOutput output;
	output.myColor.rgba = metalnessTexture.Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}

PixelOutput PixelShader_Albedo(VertexToPixel input)
{
	PixelOutput output;
	output.myColor.rgba = instanceTexture.Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}

PixelOutput PixelShader_Lambert(VertexToPixel input, LightCalculationData lightInput)
{
	float3 normal = normalTexture.Sample(textureSampler, input.myUV.xy).rgb;
	
	normal = (normal * 2) - 1;
	normal = mul(normal, input.myTangentSpaceMatrix);
	
	float3 toLight = lightInput.toLight;
	float attenuation = saturate(dot(normal.xyz, toLight));

	PixelOutput returnMe;
	returnMe.myColor = float4(attenuation, attenuation, attenuation, attenuation);
	return returnMe;
}

PixelOutput PixelShader_Fresnel(VertexToPixel input, LightCalculationData lightInput)
{
	float3 toLight = lightInput.toLight;
	float3 toEye = normalize(viewPosition.xyz - input.myWorldPosition.xyz);
	float3 substance = PixelShader_Substance(input).myColor.xyz;
	float3 halfvec = normalize(toLight + toEye.xyz);

	float LdotH = dot(toLight, halfvec);
	LdotH = saturate(LdotH);
	LdotH = 1.0f - LdotH;
	LdotH = pow(LdotH, 5);
	float3 fresnel = LdotH * (1.f - substance);
	fresnel = substance + fresnel;

	PixelOutput output;
	output.myColor.xyz = fresnel.xyz;
	output.myColor.a = 1.0f;

	return output;
}

PixelOutput PixelShader_Substance(VertexToPixel input)
{
	float4 albedo = PixelShader_Albedo(input).myColor.rgba;
	float3 metalness = PixelShader_Metalness(input).myColor.xyz;
	float3 substance = (float3(0.04f, 0.04f, 0.04f) - (float3(0.04f, 0.04f, 0.04f) * metalness)) + albedo.rgb * metalness;

	PixelOutput output;
	output.myColor.xyz = substance.rgb;
	output.myColor.a = albedo.a;
	return output;
}

PixelOutput PixelShader_Emissive(VertexToPixel input)
{
	PixelOutput output;
	output.myColor = emissiveTexture.Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}

PixelOutput PixelShader_DirectSpecularity(VertexToPixel input, LightCalculationData lightInput)
{
	float3 lightColor = lightInput.LightColor.xyz;

	float3 lambert = PixelShader_Lambert(input, lightInput).myColor.xxx;
	float3 fresnel = PixelShader_Fresnel(input, lightInput).myColor.xyz;
	float3 distribution = PixelShader_Distribution(input, lightInput).myColor.xxx;
	float3 visibility = PixelShader_Visibility(input, lightInput).myColor.xxx;

	PixelOutput output;
	output.myColor.rgb = lightColor * lambert * fresnel * distribution *visibility;
	output.myColor.a = 1.0f;
	return output;
}

PixelOutput PixelShader_Distribution(VertexToPixel input, LightCalculationData lightInput)
{
	float3 toLight = lightInput.toLight.xyz;

	float roughness = PixelShader_Roughness(input).myColor.x;
	float3 normal = PixelShader_ObjectNormal(input).myColor.xyz;
	float3 toEye = normalize(viewPosition.xyz - input.myWorldPosition.xyz);
	float3 halfvec = normalize(toLight + toEye.xyz);
	float HdotN = saturate(dot(halfvec, normal));
	
	float m = roughness * roughness;
	float m2 = m*m;
	float denominator = HdotN * HdotN * (m2 - 1.f) + 1.f;
	float distribution = m2 / (3.14159 * denominator * denominator);

	PixelOutput output;
	output.myColor.rgb = distribution.xxx;
	output.myColor.a = 1.0f;
	return output;
}

PixelOutput PixelShader_Roughness(VertexToPixel input) 
{
	PixelOutput output;
	output.myColor.rgba = roughnessTexture.Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}

PixelOutput PixelShader_ObjectNormal(VertexToPixel input)
{
	float3 normal = normalTexture.Sample(textureSampler, input.myUV.xy).rgb;

	normal = (normal * 2) - 1;
	normal = mul(normal, input.myTangentSpaceMatrix);

	PixelOutput returnMe;
	returnMe.myColor = float4(normal, 1.0f);

	return returnMe;
}

PixelOutput PixelShader_Visibility(VertexToPixel input, LightCalculationData lightInput)
{
	float3 toLight = lightInput.toLight.xyz;

	float roughness = PixelShader_Roughness(input).myColor.x;
	float roughnessRemapped = (roughness + 1.0f) / 2.0f;
	float3 normal = PixelShader_ObjectNormal(input).myColor.xyz;
	float3 toEye = normalize(viewPosition.xyz - input.myWorldPosition.xyz);

	float NdotL = saturate(dot(normal, toLight));
	float NdotV = saturate(dot(normal, toEye));

	float k = roughnessRemapped * roughnessRemapped * 1.7724f;
	float G1V = NdotV * (1.f - k) + k;
	float G1L = NdotL * (1.f - k) + k;
	float visibility = (NdotV * NdotL) / (G1V * G1L);


	PixelOutput output;
	output.myColor.rgb = visibility.xxx;
	output.myColor.a = 1.0f;
	return output;
}

PixelOutput PixelShader_AmbientDiffuse(VertexToPixel input)
{
	float3 normal = PixelShader_ObjectNormal(input).myColor.xyz;
	float3 metalnessalbedo = PixelShader_Metalnessalbedo(input).myColor.rgb;
	float3 ambientOcclusion = PixelShader_AmbientOcclusion(input).myColor.xxx;

	float3 ambientLight = EnvironmentCubemapTexture.SampleLevel(textureSampler, normal.xyz, (uint)11 - 2).xyz;
	float3 fresnel = PixelShader_ReflectionFresnel(input).myColor.xyz;

	PixelOutput output;
	output.myColor.rgb = metalnessalbedo * ambientLight * ambientOcclusion * (float3(1.0f, 1.0f, 1.0f) - fresnel);
	output.myColor.a = 1.0f;
	return output;
}

PixelOutput PixelShader_AmbientOcclusion(VertexToPixel input)
{
	PixelOutput output;
	output.myColor.rgba = ambientTexture.Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}

PixelOutput PixelShader_ReflectionFresnel(VertexToPixel input)
{
	float roughness = PixelShader_Roughness(input).myColor.x;
	float3 substance = PixelShader_Substance(input).myColor.xyz;
	float3 normal = PixelShader_ObjectNormal(input).myColor.xyz;
	float3 toEye = normalize(viewPosition.xyz - input.myWorldPosition.xyz);
	float VdotN = dot(toEye.xyz, normal);

	VdotN = saturate(VdotN);
	VdotN = 1.0f - VdotN;
	VdotN = pow(VdotN, 5);

	float3 fresnel = VdotN * (1.f - substance);
	fresnel = fresnel / (6 - 5 * roughness);
	fresnel = substance + fresnel;

	PixelOutput output;
	output.myColor.rgb = fresnel.xyz;
	output.myColor.a = 1.0f;
	return output;

}

float RoughToSPow(float fRoughness)
{
	return (2.f / (fRoughness * fRoughness)) - 2.f;
}

static const float k0 = 0.00098f;
static const float k1 = 0.9921f;
static const float fakeLysMaxSpecularPower = (2.f / (0.0014f * 0.0014f)) - 2.f;
static const float fMaxT = (exp2(-10.f / sqrt((2.f / (0.0014f * 0.0014f)) - 2.f)) - 0.00098f) / 0.9921f;

float GetSpecPowToMip(float fSpecPow, int nMips)
{
	float fSmulMaxT = (exp2(-10.0 / sqrt(fSpecPow)) - k0) / k1;

	return float(nMips - 1 - 0) * (1.0 - clamp(fSmulMaxT / fMaxT, 0.0, 1.0));
}

PixelOutput PixelShader_AmbientSpecularity(VertexToPixel input)
{
	float3 normal = PixelShader_ObjectNormal(input).myColor.xyz;
	float roughness = PixelShader_Roughness(input).myColor.x;
	float3 ambientOcclusion = PixelShader_AmbientOcclusion(input).myColor.xxx;

	float3 toEye = normalize(viewPosition.xyz - input.myWorldPosition.xyz);
	float3 reflectionVector = -reflect(toEye, normal);

	float fakeLysSpecularPower = RoughToSPow(roughness);
	float lysMipMap = GetSpecPowToMip(fakeLysSpecularPower, (uint)11);

	float3 ambientLight = EnvironmentCubemapTexture.SampleLevel(textureSampler, reflectionVector.xyz, lysMipMap).xyz;
	float3 fresnel = PixelShader_ReflectionFresnel(input).myColor.xyz;

	PixelOutput output;
	output.myColor.rgb = ambientLight * ambientOcclusion * fresnel;
	output.myColor.a = 1.0f;
	return output;

}
