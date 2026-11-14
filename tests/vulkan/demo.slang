struct Vertex {
    float3 position;
    float2 uv;
};

Sampler2D textures[];

struct ShaderData {
    float4x4 projection;
    float4x4 view;
    float4x4 model;
    Vertex *vertices;
};

struct VSOutput {
    float4 position : SV_Position;
    uint32_t instanceIndex;
    float2 uv;
};

[shader("vertex")]
VSOutput vertMain(uint instanceIndex: SV_VulkanInstanceID, uint vertexIndex: SV_VertexID, uniform ShaderData *shaderData) {
    VSOutput output;
    Vertex vertex = shaderData->vertices[vertexIndex];
    output.position = mul(shaderData->projection, mul(shaderData->view, mul(shaderData->model, float4(vertex.position, 1.0))));
    // output.position = float4(vertex.position, 1.0f);
    // output.position = mul(shaderData->model, float4(vertex.position, 1.0f));
    // output.position = mul(shaderData->view, mul(shaderData->model, float4(vertex.position, 1.0f)));
    output.instanceIndex = instanceIndex;
    output.uv = vertex.uv;
    return output;
}

[shader("fragment")]
float4 fragMain(VSOutput input) {
    return textures[NonUniformResourceIndex(input.instanceIndex)].Sample(input.uv);
}