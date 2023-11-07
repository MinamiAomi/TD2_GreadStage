struct VSInput {
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    float width : WIDTH0; 
};

struct VSOutput {
    float3 position : POSITION;
    float3 normal : NORMAL0;
    float width : WIDTH0;
};

VSOutput main(VSInput input) {
    VSOutput output;

    output.position = input.position;
    output.normal = input.normal;
    output.width = input.width;
    
    return output;
}