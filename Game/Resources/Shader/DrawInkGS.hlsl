cbuffer Constnat : register(b0) {
    float4x4 viewProjectionMatrix_;
    uint numVertices_;
};
StructuredBuffer<float3> localVertices_ : register(t0);


struct GSInput {
    float3 position : POSITION;
    float3 normal : NORMAL0;
    float width : WIDTH0;
};

struct GSOutput {
    float4 position : SV_POSITION;
    uint rtIndex : SV_RenderTargetArrayIndex;
};


float4x4 MakeWorldMatrix(float width, float3 y, float3 z, float3 w) {
    float3 x = normalize(cross(y, z));
    
    float4x4 worldMatrix = float4x4(
    float4(x * width, 0.0f),
    float4(y, 0.0f),
    float4(z, 0.0f),
    float4(w, 1.0f));
    return worldMatrix;
}

#define FRONT_RENDER_TARGET_INDEX 0
#define BACK_RENDER_TARGET_INDEX 1

[maxvertexcount(108)]
void main(
	line GSInput input[2],
	inout TriangleStream<GSOutput> output
) {
    float3 direction = normalize(input[1].position - input[0].position);

    float4x4 startMatrix = MakeWorldMatrix(input[0].width, input[0].normal, direction, input[0].position);
    float4x4 endMatrix = MakeWorldMatrix(input[1].width, input[1].normal, direction, input[1].position);
    startMatrix = mul(startMatrix, viewProjectionMatrix_);
    endMatrix = mul(endMatrix, viewProjectionMatrix_);
    
    float4 positions[4];
    positions[2] = mul(float4(localVertices_[numVertices_ - 1], 1.0f), startMatrix);
    positions[3] = mul(float4(localVertices_[numVertices_ - 1], 1.0f), endMatrix);
    
    for (uint i = 0; i < numVertices_; ++i) {
        positions[0] = mul(float4(localVertices_[i], 1.0f), startMatrix);
        positions[1] = mul(float4(localVertices_[i], 1.0f), endMatrix);

        float3 normal = cross(positions[1].xyz - positions[0].xyz, positions[2].xyz - positions[0].xyz);
        
        // 正面を向いてる
        uint forward = normal.z < 0 ? FRONT_RENDER_TARGET_INDEX : BACK_RENDER_TARGET_INDEX;
        uint reverse = forward ^ 1;
        
        GSOutput o;
        o.position = forward;
        o.position = positions[0];
        output.Append(o);
        o.position = positions[1];
        output.Append(o);
        o.position = positions[2];
        output.Append(o);
        o.position = positions[3];
        output.Append(o);
        output.RestartStrip();
        
        o.position = reverse;
        o.position = positions[1];
        output.Append(o);
        o.position = positions[0];
        output.Append(o);
        o.position = positions[3];
        output.Append(o);
        o.position = positions[2];
        output.Append(o);
        output.RestartStrip();
        
        positions[2] = positions[0];
        positions[3] = positions[1];
    }
    
}