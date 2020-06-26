cbuffer cbPerFrame : register(b0)
{
	float4x4 viewProj;
	float4x4 world;
};

struct VSInput {
	float3 position : SV_POSITION;
};

struct VSOutput {
	float4 position : SV_POSITION;
	float3 color : COLOR0;
};

VSOutput main(VSInput vin) {
	VSOutput vout = (VSOutput)0;
	vout.position = mul(mul(float4(vin.position, 1.0f), world), viewProj);
	vout.color = float3(1, 0, 1);
	return vout;
}
