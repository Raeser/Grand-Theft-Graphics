#include "CS_Particle.hlsli"
#include "Functions.hlsli"



PixelOutput main(GeometryToPixel input)
{
	PixelOutput output;
	float4 pos = input.myPosition;
	float2 uv = input.myUV;
	float lt = input.myLifetime;

#ifdef NOISE
	float2 noiseUV = uv;
	noiseUV.y += (lt * 0.4);
	float4 noise = noiseTexture.Sample(instanceSampler, noiseUV);
	uv -= ((noise.xy) - 0.5) * 2 * 0.1 * lt;
#endif

	float4 color = ParticleTexture.Sample(instanceSampler, uv) * triple_lerp(col0, col1, col2, lt);


#ifdef NOISE
	//color.a *= smoothstep(0,1,(1 - lt + smoothstep(0, 1, noise.r)));
	//color.a *= floor(1 - lt + smoothstep(0, 1, noise.r));
	float n = noise.x - lt;
	clip(n);
	n = clamp(n, 0, 0.2);
	color.a = lerp(0, color.a, n * 10);
	//color.a *= (1-lt)-noise.x;
#endif

	output.myColor = color;

	return output;
}