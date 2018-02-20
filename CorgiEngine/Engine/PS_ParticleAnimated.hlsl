#define ANIMATED
#include "CS_Particle.hlsli"
#include "Functions.hlsli"


PixelOutput main(GeometryToPixelAnimated input)
{
	float4 pos = input.myPosition;
	float2 uv0 = input.myTexCoord1;
	float2 uv1 = input.myTexCoord2;
	float framePercent = input.myFramePercent;
	float lt = input.myLifetime;

	
	float4 color;

#ifdef INTERPOLATE
	float4 color0 = particleTexture.Sample(instanceSampler, uv0) * triple_lerp(col0, col1, col2, lt);
	float4 color1 = particleTexture.Sample(instanceSampler, uv1) * triple_lerp(col0, col1, col2, lt);
	color = lerp(color0, color1, framePercent);
#else
	color = ParticleTexture.Sample(instanceSampler, uv0) * triple_lerp(col0, col1, col2, lt);
#endif


#ifdef NOISE
	float2 noiseUV = uv0;
	noiseUV.x *= rows;
	noiseUV.y *= columns;
	noiseUV.y += (lt * 0.2);
	float4 noise = noiseTexture.Sample(instanceSampler, noiseUV);

	float n = noise.x - lt;
	clip(n);
	n = clamp(n, 0, 0.2);
	color.a = lerp(0, color.a, n * 10);
#endif
	PixelOutput output;
	output.myColor = color;

	return output;
}
