#include "Lighting.hlsli"

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
    float3 color;
    uint useLighting;
    float3 rimLightColor;
    uint useRimLight;
    uint receiveShadow;
};
ConstantBuffer<Instance> instance_ : register(b1);

struct Material {
    float3 diffuse;
    float3 specular;
};
ConstantBuffer<Material> material_ : register(b2);

//// 平行光源
//struct DirectionalLight
//{
//    float3 direction; // 方向
//    float3 color; // 色
//    float intensity; // 強さ
//};

StructuredBuffer<DirectionalLight> directionalLights_ : register(t1);

struct CircleShadow {
    float3 position;
    float distance;
    float3 direction;
    float decay;
    float cosAngle;
    float cosFalloffStart;
};
StructuredBuffer<CircleShadow> circleShadows_ : register(t2);

Texture2D<float4> texture_ : register(t0);
SamplerState sampler_ : register(s0);

struct PSInput {
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION0;
    float3 normal : NORMAL0;
    float2 texcoord : TEXCOORD0;
};

struct PSOutput {
    float4 color : SV_TARGET0;
};

// ディザリングパターン
static const float pattern[4][4] = {
    { 0.00f, 0.53f, 0.13f, 0.66f },
    { 0.80f, 0.26f, 0.93f, 0.40f },
    { 0.20f, 0.73f, 0.06f, 0.60f },
    { 1.00f, 0.46f, 0.86f, 0.33f },
};

float3 ViewDirection() {
    float3x3 view = (float3x3) scene_.viewMatrix;
    float3x3 viewInverse;
    viewInverse._11_12_13 = view._11_21_31;
    viewInverse._21_22_23 = view._12_22_32;
    viewInverse._31_32_33 = view._13_23_33;
    
    return mul(float3(0.0f, 0.0f, -1.0f), viewInverse);
}

PSOutput main(PSInput input) {
    PSOutput output;
    output.color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // 位置
    float3 position = input.worldPosition;
    // 法線
    float3 normal = normalize(input.normal);
    // ピクセルからカメラへのベクトル 
    float3 pixelToCamera = normalize(scene_.cameraPosition - position);
    
    // ディザリング
    int2 xy = (int2) fmod(input.position, 4.0f);
    float dither = pattern[xy.y][xy.x];
    float rate = length(input.position.xy / float2(1280.0f, 720.0f) * 2.0f - 1.0f);
    float alpha = (length(scene_.cameraPosition - position) - scene_.ditheringRange) / scene_.ditheringRange;
    clip(rate + alpha - dither);
    
    
    //DirectionalLight directionalLight_;
    //directionalLight_.direction = normalize(float3(1.0f, -1.0f, 1.0f));
    //directionalLight_.intensity = 1.0f;
    //directionalLight_.color = float3(1.0f, 1.0f, 1.0f);
    
    
    float3 ambient = float3(0.1f, 0.1f, 0.1f);
    output.color.rgb += ambient;
    float3 textureColor = texture_.Sample(sampler_, input.texcoord).rgb * instance_.color;
       
    if (instance_.useLighting) {
        for (uint i = 0; i < scene_.numDirectionalLights; ++i) {
            // テクスチャの色
            // 拡散反射
            float3 diffuse = material_.diffuse * HalfLambertReflection(normal, directionalLights_[i].direction) * textureColor.rgb;
            // 鏡面反射
            float3 specular = material_.specular * BlinnPhongReflection(normal, directionalLights_[i].direction, pixelToCamera, 10.0f);
            // シェーディングによる色
            output.color.rgb += (diffuse + specular) * directionalLights_[i].color * directionalLights_[i].intensity;
        }
    }
    
    if (instance_.receiveShadow) {
        for (uint i = 0; i < scene_.numCircleShadows; ++i) {
            float3 spotLightDirectionOnSurface = normalize(position - circleShadows_[i].position);
            
            float distance = length(circleShadows_[i].position - position);
            float attenuationFactor = pow(saturate(-distance / circleShadows_[i].distance + 1.0f), circleShadows_[i].decay);
            float cosAngle = dot(spotLightDirectionOnSurface, circleShadows_[i].direction);
            float falloffFactor = saturate((cosAngle - circleShadows_[i].cosAngle) / (circleShadows_[i].cosFalloffStart - circleShadows_[i].cosAngle));
            
            output.color.rgb -= attenuationFactor * falloffFactor;
        }
    }
    
    output.color.a = 1.0f;
    
    
    // リムライト
    float m = saturate(1.0f - dot(normal, pixelToCamera));
    m = m * m * m * m * m;
    output.color.rgb = lerp(output.color.rgb, instance_.rimLightColor, lerp(0.0f, m, (float) instance_.useRimLight));
    
    return output;
}