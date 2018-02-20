#include "CS_Fullscreen.hlsli"


float3 Uncharted2Tonemap(float3 x)
{
	float A = 0.15;
	float B = 0.50;
	float C = 0.10;
	float D = 0.20;
	float E = 0.02;
	float F = 0.30;

	return ((x*(A*x + C*B) + D*E) / (x*(A*x + B) + D*F)) - E / F;
}


PixelOutput main(VertexToPixel input)
{
	
	float3 LDR = FullscreenTextures[0].SampleLevel(instanceSampler, input.myUV, 0).rgb;
	Uncharted2Tonemap(LDR);
	//LDR.r = LDR.r / (LDR.r + 1.f);
	//LDR.g = LDR.g / (LDR.g + 1.f);
	//LDR.b = LDR.b / (LDR.b + 1.f);
	
	PixelOutput returnMe;
	returnMe.myColor.rgb = LDR;
	returnMe.myColor.a = 1.0f;
	return returnMe;
}