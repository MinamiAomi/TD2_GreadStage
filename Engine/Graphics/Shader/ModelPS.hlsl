#include "Lighting.hlsli"

struct Scene {
    float4x4 viewProjMatrix;
    float3 cameraPosition;
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

PSOutput main(PSInput input) {
    // 位置
    float3 position = input.worldPosition;
    // 法線
    float3 normal = normalize(input.normal);
    // ピクセルからカメラへのベクトル 
    float3 pixelToCamera = normalize(scene_.cameraPosition - position);
    
    DirectionalLight directionalLight_;
    directionalLight_.direction = normalize(float3(1.0f, -1.0f, 0.0f));
    directionalLight_.intensity = 1.0f;
    directionalLight_.color = float3(1.0f, 1.0f, 1.0f);
    
    // テクスチャの色
    float3 textureColor = texture_.Sample(sampler_, input.texcoord).rgb * instance_.color;
    // 拡散反射
    float3 diffuse = material_.diffuse * HalfLambertReflection(normal, directionalLight_.direction);
    // 鏡面反射
    float3 specular = material_.specular * BlinnPhongReflection(normal, directionalLight_.direction, pixelToCamera, 10.0f);
    // シェーディングによる色
    float3 shadeColor = (diffuse + specular) * directionalLight_.color * directionalLight_.intensity;
    // ライティングを使用しない場合テクスチャの色をそのまま使う
    shadeColor = lerp(float3(1.0f, 1.0f, 1.0f), shadeColor, float(instance_.useLighting));
       
    PSOutput output;
    output.color.rgb = textureColor * shadeColor;
    output.color.a = 1.0f;
   
   // output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    
    // フレネル
//    float m = saturate(1.0f - dot(normal, pixelToCamera));
//    output.color.rgb = lerp(float3(1.0f, 0.0f, 0.0f), output.color.rgb, step((m * m * m * m * m), 0.4f));
    
    //output.color.rgb = specular;
    
    return output;
}