struct PSInput {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

float4 main(PSInput pin) : SV_TARGET
{
	return float4(pin.normal, 1.0);
}
