cbuffer cbPerFrame : register(b0)
{
	float4x4 viewProj;
	float4x4 world;
};

struct VSInput {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct VSOutput {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

VSOutput main(VSInput vin) {
	VSOutput vout = (VSOutput)0;
	vout.position = mul(mul(float4(vin.position, 1.0f), world), viewProj);
	vout.normal = abs(vin.normal);
	vout.uv = vin.uv;
	return vout;
}
