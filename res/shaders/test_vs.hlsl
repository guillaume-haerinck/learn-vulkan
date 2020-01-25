static float2 positions[3] = {
    float2(0.0, -0.5),
    float2(0.5, 0.5),
    float2(-0.5, 0.5)
};

static float3 colors[3] = {
    float3(1.0, 0.0, 0.0),
    float3(0.0, 1.0, 0.0),
    float3(0.0, 0.0, 1.0)
};

struct VSInput {
	uint index : SV_VertexID;
};

struct VSOutput {
	float4 position : SV_POSITION;
	float3 color : COLOR0;
};

VSOutput main(VSInput vin) {
	VSOutput vout = (VSOutput)0;
	vout.position = float4(positions[vin.index], 0.0, 1.0);
	vout.color = colors[vin.index];
	return vout;
}
