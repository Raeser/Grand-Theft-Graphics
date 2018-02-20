#include "CS_DEFLightRendering.hlsli"

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
PixelOutput PixelShader_Roughness(VertexToPixel input);
PixelOutput PixelShader_ObjectNormal(VertexToPixel input);
PixelOutput PixelShader_WorldPos(VertexToPixel input);

PixelOutput PixelShader_CalculatePBR(VertexToPixel input, LightCalculationData lightInput)
{
	float4 PreCalculated = DeferredTextures[5].Sample(textureSampler, input.myUV.xy).rgba;
	float3 directDiffuse = PixelShader_DirectDiffuse(input, lightInput).myColor.rgb;
	float3 directSpecularity = PixelShader_DirectSpecularity(input, lightInput).myColor.rgb;

	PixelOutput returnMe;
	returnMe.myColor.rgb = PreCalculated.rgb + directDiffuse + directSpecularity;
	returnMe.myColor.a = PixelShader_Albedo(input).myColor.a;


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
	output.myColor.rgba = DeferredTextures[3].Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}

PixelOutput PixelShader_Albedo(VertexToPixel input)
{
	PixelOutput output;
	output.myColor.rgba = DeferredTextures[1].Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}

PixelOutput PixelShader_Lambert(VertexToPixel input, LightCalculationData lightInput)
{
	float3 normal = PixelShader_ObjectNormal(input).myColor.rgb;

	float3 toLight = lightInput.toLight;
	float attenuation = saturate(dot(normal.xyz, toLight));

	PixelOutput returnMe;
	returnMe.myColor = float4(attenuation, attenuation, attenuation, attenuation);
	return returnMe;
}

PixelOutput PixelShader_Fresnel(VertexToPixel input, LightCalculationData lightInput)
{
	float3 toLight = lightInput.toLight;
	float3 toEye = normalize(viewPosition.xyz - PixelShader_WorldPos(input).myColor.xyz);
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
	float3 toEye = normalize(viewPosition.xyz - PixelShader_WorldPos(input).myColor.xyz);
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
	output.myColor.rgba = DeferredTextures[4].Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}

PixelOutput PixelShader_ObjectNormal(VertexToPixel input)
{
	float4 normal = DeferredTextures[2].Sample(textureSampler, input.myUV.xy).rgba;

	PixelOutput returnMe;
	returnMe.myColor = normal;

	return returnMe;
}

PixelOutput PixelShader_Visibility(VertexToPixel input, LightCalculationData lightInput)
{
	float3 toLight = lightInput.toLight.xyz;

	float roughness = PixelShader_Roughness(input).myColor.x;
	float roughnessRemapped = (roughness + 1.0f) / 2.0f;
	float3 normal = PixelShader_ObjectNormal(input).myColor.xyz;
	float3 toEye = normalize(viewPosition.xyz - PixelShader_WorldPos(input).myColor.xyz);

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

PixelOutput PixelShader_WorldPos(VertexToPixel input)
{
	PixelOutput output;
	output.myColor.rgba = DeferredTextures[0].Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}