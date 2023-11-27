#include "Convert.hlsli"

struct Instance {
    float4x4 worldMatrix;
    float seed;
};
StructuredBuffer<Instance> instances_ : register(t0);

struct PSInput {
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    uint instanceID : SV_InstanceID;
};

struct PSOutput {
    float4 color : SV_TARGET0;
};

static const float PI = 3.1415926535f;

PSOutput main(PSInput input) {
    
    PSOutput output;

    output.color.rgb = HSVToRGB(float3(instances_[input.instanceID].seed, 1.0f, 1.0f));
    output.color.a = 1.0f;
    
    return output;
}