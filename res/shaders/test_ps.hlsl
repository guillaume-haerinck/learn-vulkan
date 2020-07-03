Texture2D tex : register(t1);
SamplerState samplr : register(s1);

struct PSInput {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

float4 main(PSInput pin) : SV_TARGET
{
	return tex.Sample(samplr, pin.uv);
}
