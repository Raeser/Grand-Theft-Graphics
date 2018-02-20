#include "CS_Water.hlsli"
//for water

struct UVPanner
{
	float2 pannedUV;
};

UVPanner UVPannerFunction(float2 UV, float Time, float PanU, float PanV)
{
	UVPanner OUT;
	OUT.pannedUV = float2(1,0);
	
	float mulPanU = (Time * PanU);
	float mulPanV = (Time * PanV);
	OUT.pannedUV = float2(UV + float2(mulPanU, mulPanV));
	
	return OUT;
}

ColorOutput SampleSplatMap(VertexToPixel input)
{
	ColorOutput output;
	output.myColor.rgba = SplatmapTexture.Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}

ColorOutput SampleAlbedo(VertexToPixel input)
{
	ColorOutput output;
	UVPanner panner = UVPannerFunction(input.myUV.xy, DTHACK * 0.025f, 0.723f, 0.323f); //tested these floats in maya i dunno man 
	output.myColor.rgba = albedoTexture.Sample(textureSampler, float2(panner.pannedUV.x, 1 - panner.pannedUV.y)).rgba; //
	return output;
}

ColorOutput SecondSampleAlbedo(VertexToPixel input)
{
	ColorOutput output;
	UVPanner panner = UVPannerFunction(input.myUV.xy, DTHACK * 0.025f, 0.737f, -0.337f); //tested these floats in maya i dunno man 
	output.myColor.rgba = albedoTexture.Sample(textureSampler, float2(panner.pannedUV.x, 1 - panner.pannedUV.y)).rgba; //
	return output;
}


ColorOutput SampleDirtTexture(VertexToPixel input)
{
	ColorOutput output;
	UVPanner panner = UVPannerFunction(input.myUV.xy, DTHACK * 0.1f, 1.0f,-0.42f); //tested these floats in maya i dunno man
	output.myColor.rgba = DirtTexture.Sample(textureSampler, float2(panner.pannedUV.x, 1-panner.pannedUV.y)).rgba;
	return output;
}

ColorOutput SampleSandTexture(VertexToPixel input)
{
	ColorOutput output;
	output.myColor.rgba = SandTexture.Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}

ColorOutput SampleMossyTexture(VertexToPixel input)
{
	ColorOutput output;
	output.myColor.rgba = MossyTexture.Sample(textureSampler, input.myUV.xy).rgba;
	return output;
}

ColorOutput PixelShader_ObjectNormal(VertexToPixel input)
{
	UVPanner panner = UVPannerFunction(input.myUV.xy, DTHACK * 0.1f, 0.723f, 0.323f);
	float3 normal = normalTexture.Sample(textureSampler, float2(panner.pannedUV.x, 1 - panner.pannedUV.y)).rgb;

	normal = (normal * 2) - 1;
	normal = mul(normal, input.myTangentSpaceMatrix);

	ColorOutput returnMe;
	returnMe.myColor = float4(normal, 1.0f);

	return returnMe;
}

ColorOutput PixelShader_ObjectNormal_test(VertexToPixel input)
{
	UVPanner panner = UVPannerFunction(input.myUV.xy, DTHACK * 0.1f, 0.737f, -0.337f);
	float3 normal = normalTexture.Sample(textureSampler, float2(panner.pannedUV.x, 1 - panner.pannedUV.y)).rgb;

	normal = (normal * 2) - 1;
	normal = mul(normal, input.myTangentSpaceMatrix);

	ColorOutput returnMe;
	returnMe.myColor = float4(normal, 1.0f);

	return returnMe;
}



PixelOutput main(VertexToPixel input)
{
	/*Bound Resources are as follows
		-normalTexture = The Actual Normalmap of the Model

		-albedoTexture = The Original groundsurface albedo
		-ambientTexture = Dirt texture
		-emissiveTexture = Sand Texture
		-metalnessTexture = Mossy Texture
		-roughnessTexture = SplatMap Texture
	*/

	//float4 SplatMapOutput = SampleSplatMap(input).myColor;
	//float4 DirtOutput = SampleDirtTexture(input).myColor;
	//float4 SandOutput = SampleSandTexture(input).myColor;
	//float4 MossyOutput = SampleMossyTexture(input).myColor;
	//float4 albedoOutput = SampleAlbedo(input).myColor;

	float4 normalD = PixelShader_ObjectNormal(input).myColor;
	float4 normalU = PixelShader_ObjectNormal_test(input).myColor;
	float4 normalOutput = lerp(normalD, normalU, 0.5f);
	
	float4 albedoD = SampleAlbedo(input).myColor;
	float4 albedoU = SecondSampleAlbedo(input).myColor;
	float4 albedoOutput = lerp(albedoD, albedoU, 0.5f);



	//float4 waterOutput = lerp(albedoOutput, DirtOutput, 0.5f);
	float4 waterOutput = float4(0.19f, 0.5f,  0.99f, 1.f);
	
	PixelOutput returnMe;
	returnMe.myWorldPosition = input.myWorldPosition; //dont touch pls
	returnMe.myNormals = float4(1,1,1,1); //PixelShader_ObjectNormal(input).myColor; //normals is gud
	
	returnMe.myAlbedo = albedoOutput; //float4(0.6f * DTHACK, 0.2f * DTHACK, 0.f, 1); //lerp all textures into this albedo pixel
	
	returnMe.myMetallic = float4(0,0,0,0); //PixelShader_Metalness(input).myColor; 
	returnMe.myRoughness = float4(0,0,0,0); //PixelShader_Roughness(input).myColor;
	returnMe.myEmissiveAmbSpecAmbDiff = float4(0,0,0,0); //fuck what you just did!
	

	//only Albedo makes a difference in this shithole so make the others return 0,0,0,0
	
	
	return returnMe;
}
