#include "Random.hlsli"

struct Instance {
    float4x4 worldMatrix;
    float seed;
};
StructuredBuffer<Instance> instances_ : register(t0);

struct Common {
    float4x4 viewProjectionMatrix;
    float3 cameraPosition;
    float time;
};
ConstantBuffer<Common> common_ : register(b0);

struct VSInput {
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    uint instanceID : SV_InstanceID;
};

struct VSOutput {
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    uint instanceID : SV_InstanceID;
};

static const float PI = 3.1415926535f;

VSOutput main(VSInput input) {
    
    VSOutput output;

    float t = frac(common_.time + Random(Random(instances_[input.instanceID].seed)));
    t = 1.0f - (abs(t - 0.5f) * 2.0f);
    float3 position = input.position * lerp(0.5f, 2.0f, t);
    position = mul(float4(position, 1.0f), instances_[input.instanceID].worldMatrix).xyz + common_.cameraPosition;
    output.position = mul(float4(position, 1.0f), common_.viewProjectionMatrix);
    output.texcoord = input.texcoord;
    output.instanceID = input.instanceID; 
    
    return output;
}