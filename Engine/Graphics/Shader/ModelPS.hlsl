#include "Lighting.hlsli"

struct Scene {
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float3 cameraPosition;
    float ditheringRange;
};
ConstantBuffer<Scene> scene_ : register(b0);

struct Instance {
    float4x4 worldMatrix;
    float3 color;
    uint useLighting;
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

//ConstantBuffer<DirectionalLight> directionalLight_ : register(b3);

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

static const float pattern[4][4] = {
    { 0.00f, 0.53f, 0.13f, 0.66f },
    { 0.80f, 0.26f, 0.93f, 0.40f },
    { 0.20f, 0.73f, 0.06f, 0.60f },
    { 1.00f, 0.46f, 0.86f, 0.33f },
};

float3 ViewDirection() {
    float3x3 view = (float3x3)scene_.viewMatrix;
    float3x3 viewInverse;
    viewInverse._11_12_13 = view._11_21_31;
    viewInverse._21_22_23 = view._12_22_32;
    viewInverse._31_32_33 = view._13_23_33;
    
    return mul(float3(0.0f, 0.0f, -1.0f), viewInverse);
}

PSOutput main(PSInput input) {
    // 位置
    float3 position = input.worldPosition;
    // 法線
    float3 normal = normalize(input.normal);
    // ピクセルからカメラへのベクトル 
    float3 pixelToCamera = normalize(scene_.cameraPosition - position);
    
    int2 xy = (int2)fmod(input.position, 4.0f);
    float dither = pattern[xy.y][xy.x];
    
    float rate = length(input.position.xy / float2(1280.0f, 720.0f) * 2.0f - 1.0f);
    float alpha = (length(scene_.cameraPosition - position) - scene_.ditheringRange) / scene_.ditheringRange;
    clip(rate + alpha - dither);
    
    
    DirectionalLight directionalLight_;
    directionalLight_.direction = normalize(ViewDirection());
    directionalLight_.intensity = 1.0f;
    directionalLight_.color = float3(1.0f, 1.0f, 1.0f);
    
    float3 ambient = float3(0.1f, 0.1f, 0.1f);
    // テクスチャの色
    float3 textureColor = texture_.Sample(sampler_, input.texcoord).rgb * instance_.color;
    // 拡散反射
    float3 diffuse = material_.diffuse * HalfLambertReflection(normal, directionalLight_.direction);
    // 鏡面反射
    float3 specular = material_.specular * BlinnPhongReflection(normal, directionalLight_.direction, pixelToCamera, 10.0f);
    // シェーディングによる色
    float3 shadeColor = (diffuse + specular + ambient) * directionalLight_.color * directionalLight_.intensity;
    // ライティングを使用しない場合テクスチャの色をそのまま使う
    shadeColor = lerp(float3(1.0f, 1.0f, 1.0f), shadeColor, float(instance_.useLighting));
       
    PSOutput output;
    output.color.rgb = textureColor * shadeColor;
    output.color.a = 1.0f;

    //float b = length((input.position.xy / float2(1280.0f, 720.0f)) * 2.0f - 1.0f);
    //output.color.rgb = b;
    
    //output.color.rg = input.position.xy / float2(1280.0f, 720.0f) * 2.0f -0.5f;
    //output.color.b = 0.0f;
    
   // output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    
    // フレネル
    //float m = saturate(1.0f - dot(normal, pixelToCamera));
    //output.color.rgb += lerp(float3(0.0f, 0.0f, 0.0f), float3(0.5f,0.5f,0.5f), (m * m * m * m * m));
    
    //output.color.rgb = specular;
    
    return output;
}