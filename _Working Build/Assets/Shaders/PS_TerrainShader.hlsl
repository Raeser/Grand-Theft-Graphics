#include "CS_TerrainShader.hlsli"
//for GroundPlane

#define Tex11End float2(0.5f, 0.5f)
#define Tex12End float2(1.0f, 0.5f)
#define Tex21End float2(0.5f, 1.0f)
#define Tex22End float2(1.0f, 1.0f)

ColorOutput SampleSplatMap(float2 UV)
{
	ColorOutput output;
	output.myColor.rgba = SplatmapTexture.Sample(textureSamplerWrap, UV.xy).rgba;
	return output;
}

ColorOutput SampleAlbedo(float2 UV)
{
	ColorOutput output;
	output.myColor.rgba = albedoTexture.Sample(textureSamplerWrap, UV.xy).rgba;
	return output;
}

ColorOutput SampleNormals(VertexToPixel input, float2 UV)
{
	float3 normal = normalTexture.Sample(textureSamplerWrap, UV.xy).rgb;

	normal = (normal * 2) - 1;
	normal = mul(normal, input.myTangentSpaceMatrix);

	ColorOutput returnMe;
	returnMe.myColor = float4(normal, 1.0f);

	return returnMe;
}

ColorOutput SampleRougness(float2 UV)
{
	ColorOutput output;
	output.myColor.rgba = roughnessTexture.Sample(textureSamplerWrap, UV.xy).rgba;
	return output;
}

ColorOutput SampleAmbientOcclusion(float2 UV)
{
	ColorOutput output;
	output.myColor.rgba = ambientTexture.Sample(textureSamplerWrap, UV.xy).rgba;
	return output;
}

ColorOutput SampleEmissive(float2 UV)
{
	ColorOutput output;
	output.myColor.rgba = emissiveTexture.Sample(textureSamplerWrap, UV.xy).rgba;
	return output;
}




ColorOutput ReflectionFresnel(VertexToPixel input, float4 inRoughness, float4 inNormal)
{
	float roughness = inRoughness.x;
	float3 substance = float3(0.04f, 0.04f, 0.04f);
	float3 normal = inNormal.xyz;
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

ColorOutput AmbientSpecularity(VertexToPixel input, float4 inNormal, float4 inRoughness, float4 ambientOcc )
{
	float3 normal = inNormal.xyz;
	float roughness = inRoughness.x;
	float3 ambientOcclusion = ambientOcc.xxx;

	float3 toEye = normalize(viewPosition.xyz - input.myWorldPosition.xyz);
	float3 reflectionVector = -reflect(toEye, normal);

	float fakeLysSpecularPower = RoughToSPow(roughness);
	float lysMipMap = GetSpecPowToMip(fakeLysSpecularPower, (uint)11);

	float3 ambientLight = EnvironmentCubemapTexture.SampleLevel(textureSampler, reflectionVector.xyz, lysMipMap).xyz;
	float3 fresnel = ReflectionFresnel(input, inRoughness, inNormal).myColor.xyz;

	ColorOutput output;
	output.myColor.rgb = ambientLight * ambientOcclusion * fresnel;
	output.myColor.a = 1.0f;
	return output;

}


#define SplatMapSize 128.f
#define AntiWrap 2.f / 512.f

float4 SplatMap(float4 aSplatmap, float4 a, float4 b, float4 c, float4 d)
{
	float4 Out = float4(0, 0, 0, 1);

	Out += a * aSplatmap.r;
	Out += b * aSplatmap.g;
	Out += c * aSplatmap.b;
	Out += d * aSplatmap.a;

	return Out;
}

float2 ClampUV(float2 aUVToClamp, float2 aMinValue, float2 aMaxValue)
{
	float2 returnMe;
	returnMe.x = clamp(aUVToClamp.x, aMinValue.x, aMaxValue.x);
	returnMe.y = clamp(aUVToClamp.y, aMinValue.y, aMaxValue.y);
	return returnMe;
}

PixelOutput main(VertexToPixel input)
{

	float4 SplatMapOutput = SampleSplatMap(float2(-input.myUV.y, -input.myUV.x)).myColor; //anti mirroring and anti wrong rotation -> float2(-input.myUV.y, -input.myUV.x)
	float sum = SplatMapOutput.x + SplatMapOutput.y + SplatMapOutput.z + SplatMapOutput.w;
	SplatMapOutput = SplatMapOutput / sum;

	float2 uv = input.myUV * SplatMapSize;
	float2 PixelPosition = floor(uv);

	float2 UVTileTexture = (uv - PixelPosition) / 2.f;
	
	float myNumberOfPixels = 256.f;
	float PixelSize = 1.f / (myNumberOfPixels);

	UVTileTexture = UVTileTexture / myNumberOfPixels;
	UVTileTexture = UVTileTexture * (myNumberOfPixels - 4.f);
	UVTileTexture.x = UVTileTexture.x + PixelSize;
	UVTileTexture.y = UVTileTexture.y + PixelSize;


	float2 texture1UV = UVTileTexture;
	float2 texture2UV = UVTileTexture + float2(0.5f, 0.f);
	float2 texture3UV = UVTileTexture + float2(0.f, 0.5f);
	float2 texture4UV = UVTileTexture + float2(0.5f, 0.5f);


	float4 a_albedo = SampleAlbedo(texture1UV).myColor;
	float4 b_albedo = SampleAlbedo(texture2UV).myColor;
	float4 c_albedo = SampleAlbedo(texture3UV).myColor;
	float4 d_albedo = SampleAlbedo(texture4UV).myColor;
	float4 OutAlbedo = SplatMap(SplatMapOutput, a_albedo, b_albedo, c_albedo, d_albedo);

	float4 a_normal = SampleNormals(input, texture1UV).myColor;
	float4 b_normal = SampleNormals(input, texture2UV).myColor;
	float4 c_normal = SampleNormals(input, texture3UV).myColor;
	float4 d_normal = SampleNormals(input, texture4UV).myColor;
	float4 OutNormal = SplatMap(SplatMapOutput, a_normal, b_normal, c_normal, d_normal);

	float4 a_roughness = SampleRougness(texture1UV).myColor;
	float4 b_roughness = SampleRougness(texture2UV).myColor;
	float4 c_roughness = SampleRougness(texture3UV).myColor;
	float4 d_roughness = SampleRougness(texture4UV).myColor;
	float4 OutRoughness = SplatMap(SplatMapOutput, a_roughness, b_roughness, c_roughness, d_roughness);


	float4 a_emissive = SampleEmissive(texture1UV).myColor; 
	float4 b_emissive = SampleEmissive(texture2UV).myColor;
	float4 c_emissive = SampleEmissive(texture3UV).myColor;
	float4 d_emissive = SampleEmissive(texture4UV).myColor;
	float4 OutEmissive = SplatMap(SplatMapOutput, a_emissive, b_emissive, c_emissive, d_emissive);

	float3 emissive = OutEmissive.rgb;

	float4 a_AO = SampleAmbientOcclusion(texture1UV).myColor;
	float4 b_AO = SampleAmbientOcclusion(texture2UV).myColor;
	float4 c_AO = SampleAmbientOcclusion(texture3UV).myColor;
	float4 d_AO = SampleAmbientOcclusion(texture4UV).myColor;
	float4 OutAmbientOccl = SplatMap(SplatMapOutput, a_AO, b_AO, c_AO, d_AO); 


	float3 normal = OutNormal.xyz;
	float3 ambientOcclusion = OutAmbientOccl.xxx;

	float3 ambientLight = EnvironmentCubemapTexture.SampleLevel(textureSampler, normal.xyz, (uint)11 - 2).xyz;
	float3 fresnel = ReflectionFresnel(input, OutRoughness, OutNormal).myColor.xyz;

	
	float3 ambientDiffuse = OutAlbedo.xyz * ambientLight * ambientOcclusion * (float3(1.0f, 1.0f, 1.0f) - fresnel);

	float3 ambientSpecularity = AmbientSpecularity(input, OutNormal, OutRoughness, OutAmbientOccl).myColor.rgb;

	PixelOutput returnMe;
	returnMe.myWorldPosition = input.myWorldPosition;
	returnMe.myNormals = OutNormal;
	returnMe.myAlbedo = OutAlbedo;
	returnMe.myRoughness = OutRoughness;
	returnMe.myEmissiveAmbSpecAmbDiff.rgb = ambientDiffuse + ambientSpecularity + emissive;
	returnMe.myEmissiveAmbSpecAmbDiff.a = 1.f;
	returnMe.myMetallic = float4(0,0,0,0);
	
	return returnMe;
}


