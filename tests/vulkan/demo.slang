struct VSInput {
    float3 inPosition;
    float3 inColor;
    float2 inTextureCoordinates;
}

struct UniformBuffer {
    float4x4 model;
    float4x4 view;
    float4x4 projection;
}

ConstantBuffer<UniformBuffer> ubo;

struct VSOutput {
    float4 pos : SV_Position;
    float3 color;
    float2 textureCoordinates;
}

[shader("vertex")]
VSOutput vertMain(VSInput input) {
    VSOutput output;
    output.pos = mul(ubo.projection, mul(ubo.view, mul(ubo.model, float4(input.inPosition, 1.0))));
    output.color = input.inColor;
    output.textureCoordinates = input.inTextureCoordinates;
    return output;
}

Sampler2D texture;

[shader("fragment")]
float4 fragMain(VSOutput output) : SV_Target {
    return texture.Sample(output.textureCoordinates);
    // return float4(output.textureCoordinates, 0.0, 1.0);
}