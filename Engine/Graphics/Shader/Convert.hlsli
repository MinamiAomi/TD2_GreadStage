
// https://chilliant.blogspot.com/2012/08/srgb-approximations-for-hlsl.html
float3 LinearToSRGB(in float3 rgb) {
    float3 sqrt1 = sqrt(rgb);
    float3 sqrt2 = sqrt(sqrt1);
    float3 sqrt3 = sqrt(sqrt2);
    float3 sRGB = 0.662002687f * sqrt1 + 0.684122060f * sqrt2 - 0.323583601f * sqrt3 - 0.022541147f * rgb;
    return sRGB;
}

float3 SRGBToLinear(in float3 sRGB) {
    float3 rgb = sRGB * (sRGB * (sRGB * 0.305306011f + 0.682171111f) + 0.012522878f);
    return rgb;
}

// https://web.archive.org/web/20200207113336/http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
float3 RGBToHSV(in float3 rgb) {
    float4 k = float4(0.0f, -1.0f / 3.0f, 2.0f / 3.0f, -1.0f);
    float4 p = lerp(float4(rgb.bg, k.wz), float4(rgb.gb, k.xy), step(rgb.b, rgb.g));
    float4 q = lerp(float4(p.xyw, rgb.r), float4(rgb.r, p.yzx), step(p.x, rgb.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10f;
    return float3(abs(q.z + (q.w - q.y)), 1.0f, 0.0f);

}

float3 HSVToRGB(in float3 hsv) {
    float4 k = float4(1.0f, 2.0f / 3.0f, 1.0f / 3.0f, 3.0f);
    float3 p = abs(frac(hsv.xxx + k.xyz) * 6.0f - k.www);
    return float3(0.0f, 0.0f, 0.0f);
}