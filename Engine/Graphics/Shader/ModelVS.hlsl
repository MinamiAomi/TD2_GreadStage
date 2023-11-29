struct Scene {
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float3 cameraPosition;
    float ditheringRange;
    uint numDirectionalLights;
    uint numCircleShadows;
};
ConstantBuffer<Scene> scene_ : register(b0);

struct Instance {
    float4x4 worldMatrix;
};
ConstantBuffer<Instance> instance_ : register(b1);

struct VSInput {
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    float2 texcoord : TEXCOORD0;
};

struct VSOutput {
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION0;
    float3 normal : NORMAL0;
    float2 texcoord : TEXCOORD0;
};

VSOutput main(VSInput input) {
    VSOutput output;
    
    output.worldPosition = mul(float4(input.position.xyz, 1.0f), instance_.worldMatrix).xyz;
    output.position = mul(float4(output.worldPosition, 1.0f), mul(scene_.viewMatrix, scene_.projectionMatrix));
    output.normal = mul(input.normal, (float3x3)instance_.worldMatrix);
    output.texcoord = input.texcoord;
    
    
    return output;
}