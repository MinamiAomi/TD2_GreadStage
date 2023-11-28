#include "Convert.hlsli"
#include "Random.hlsli"

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

float Asteroid(float2 xy) {
    float angle = atan2(xy.y, xy.x);
    float secant = 1.0f / cos(angle);
    float tangent = abs(tan(angle));
    float d = abs(secant) / pow(1.0f + pow(tangent, 2.0f / 3.0f), 3.0f / 2.0f);
    return d;
}

PSOutput main(PSInput input) {
    
    PSOutput output;

    float2 xy = input.texcoord * 2.0f - 1.0f;
    
    float d = Asteroid(xy);
    clip(d - length(xy));
        
    output.color.rgb = HSVToRGB(float3(Random(instances_[input.instanceID].seed), 1.0f, 1.0f));
    output.color.a = 1.0f;

    return output;
}