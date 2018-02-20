#include "CS_DEFModel.hlsli"


ColorOutput PixelShader_Metalness(VertexToPixel input)
{
	ColorOutput output;
	output.myColor.rgba = metalnessTexture.Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}

ColorOutput PixelShader_Albedo(VertexToPixel input)
{
	ColorOutput output;
	output.myColor.rgba = instanceTexture.Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}

ColorOutput PixelShader_Emissive(VertexToPixel input)
{
	ColorOutput output;
	output.myColor = emissiveTexture.Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}

ColorOutput PixelShader_Roughness(VertexToPixel input)
{
	ColorOutput output;
	output.myColor.rgba = roughnessTexture.Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}

ColorOutput PixelShader_ObjectNormal(VertexToPixel input)
{
	float3 normal = normalTexture.Sample(textureSampler, input.myUV.xy).rgb;

	normal = (normal * 2) - 1;
	normal = mul(normal, input.myTangentSpaceMatrix);

	ColorOutput returnMe;
	returnMe.myColor = float4(normal, 1.0f);

	return returnMe;
}

ColorOutput PixelShader_AmbientOcclusion(VertexToPixel input)
{
	ColorOutput output;
	output.myColor.rgba = ambientTexture.Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}


ColorOutput PixelShader_Substance(VertexToPixel input)
{
	float4 albedo = PixelShader_Albedo(input).myColor.rgba;
	float3 metalness = PixelShader_Metalness(input).myColor.xyz;
	float3 substance = (float3(0.04f, 0.04f, 0.04f) - (float3(0.04f, 0.04f, 0.04f) * metalness)) + albedo.rgb * metalness;

	ColorOutput output;
	output.myColor.xyz = substance.rgb;
	output.myColor.a = albedo.a;
	return output;
}

ColorOutput PixelShader_ReflectionFresnel(VertexToPixel input)
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

	ColorOutput output;
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

ColorOutput PixelShader_AmbientSpecularity(VertexToPixel input)
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

	ColorOutput output;
	output.myColor.rgb = ambientLight * ambientOcclusion * fresnel;
	output.myColor.a = 1.0f;
	return output;

}

ColorOutput PixelShader_Metalnessalbedo(VertexToPixel input)
{
	float4 albedo = PixelShader_Albedo(input).myColor.rgba;
	float3 metalness = PixelShader_Metalness(input).myColor.xyz;
	float3 metalnessalbedo = albedo.rgb - (albedo.rgb * metalness);

	//if (albedo.a <= 0.0f)
	//{
	//	discard;
	//}

	ColorOutput output;
	output.myColor.rgb = metalnessalbedo.xyz;
	output.myColor.a = albedo.a;
	return output;
}

ColorOutput PixelShader_AmbientDiffuse(VertexToPixel input)
{
	float3 normal = PixelShader_ObjectNormal(input).myColor.xyz;
	float3 metalnessalbedo = PixelShader_Metalnessalbedo(input).myColor.rgb;
	float3 ambientOcclusion = PixelShader_AmbientOcclusion(input).myColor.xxx;

	float3 ambientLight = EnvironmentCubemapTexture.SampleLevel(textureSampler, normal.xyz, (uint)11 - 2).xyz;
	float3 fresnel = PixelShader_ReflectionFresnel(input).myColor.xyz;

	ColorOutput output;
	output.myColor.rgb = metalnessalbedo * ambientLight * ambientOcclusion * (float3(1.0f, 1.0f, 1.0f) - fresnel);
	output.myColor.a = 1.0f;
	return output;
}

PixelOutput main(VertexToPixel input)
{
	PixelOutput returnMe;
	returnMe.myWorldPosition = input.myWorldPosition;
	returnMe.myAlbedo = PixelShader_Albedo(input).myColor;

	returnMe.myNormals = PixelShader_ObjectNormal(input).myColor;
	returnMe.myMetallic = PixelShader_Metalness(input).myColor;
	returnMe.myRoughness = PixelShader_Roughness(input).myColor;

	float3 ambientDiffuse = PixelShader_AmbientDiffuse(input).myColor.rgb;
	float3 ambientSpecularity = PixelShader_AmbientSpecularity(input).myColor.rgb;
	float3 emissive = PixelShader_Emissive(input).myColor.rgb;

	returnMe.myEmissiveAmbSpecAmbDiff.rgb = ambientDiffuse + ambientSpecularity + emissive;
	returnMe.myEmissiveAmbSpecAmbDiff.a = 1.f;
	return returnMe;
}
