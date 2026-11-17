struct Vertex {
    float3 position;
    float2 uv;
};

Sampler2D textures[];

// struct ShaderData {
//     float4x4 projection;
//     float4x4 view;
//     float4x4 model;
//     uint32_t textureIndex;
//     Vertex *vertices;
//     uint32_t *indices;
// };

struct FrameConstants {
    float4x4 projection;
    float4x4 view;
}

struct Position {
    float4x4 model;
};

struct PushConstants {
    FrameConstants *frameConstants;
    Position *position;
    Vertex *vertices;
    uint32_t *indices;
    uint32_t textureIndex;
};

struct VSOutput {
    float4 position : SV_Position;
    uint32_t textureIndex;
    float2 uv;
};

[[vk_push_constant]]
ConstantBuffer<PushConstants> pushConstants;

[shader("vertex")]
VSOutput vertMain(uint vertexIndex: SV_VertexID) {
    VSOutput output;
    uint32_t index = pushConstants->indices[vertexIndex];
    Vertex vertex = pushConstants->vertices[index];
    output.position = mul(pushConstants->frameConstants.projection, mul(pushConstants->frameConstants.view, mul(pushConstants.position.model, float4(vertex.position, 1.0))));
    // output.position = float4(vertex.position, 1.0f);
    // output.position = mul(pushConstants->position.model, float4(vertex.position, 1.0f));
    output.textureIndex = pushConstants.textureIndex;
    output.uv = vertex.uv;
    return output;
}

[shader("fragment")]
float4 fragMain(VSOutput input) {
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
    // return textures[NonUniformResourceIndex(input.textureIndex)].Sample(input.uv);
}