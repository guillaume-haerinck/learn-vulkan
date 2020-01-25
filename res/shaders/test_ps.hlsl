struct PSInput {
	float4 position : SV_POSITION;
	float3 color : COLOR0;
};

float4 main(PSInput pin) : SV_TARGET
{
	return float4(pin.color, 1.0);
}
