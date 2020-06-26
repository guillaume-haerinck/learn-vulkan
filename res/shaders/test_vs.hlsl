cbuffer cbPerFrame : register(b0)
{
	float4x4 viewProj;
	float4x4 world;
};

struct VSInput {
	float2 position : SV_POSITION;
	float3 color : COLOR0;
};

struct VSOutput {
	float4 position : SV_POSITION;
	float3 color : COLOR0;
};

VSOutput main(VSInput vin) {
	VSOutput vout = (VSOutput)0;
	vout.position = mul(mul(float4(vin.position, 0.0, 1.0), world), viewProj);
	vout.color = vin.color;
	return vout;
}
