struct Vertex
{
    float2 position;
    float3 color;
};

struct VSOutput
{
    float4 position : SV_Position;
    float3 color : COLOR0;
};

[shader("vertex")]
[require(spvDescriptorHeapEXT)]
VSOutput vertexMain(uint vertexIndex : SV_VertexID)
{
    // StructuredBuffer<Vertex>.Handle vertexBufferHandle = StructuredBuffer<Vertex>.Handle(uint2(0, 0));

    // StructuredBuffer<Vertex> vertices = vertexBufferHandle;
    __loadResourceDescriptorFromHeap
    // Texture2D.Handle()
    Vertex vertex = vertices[vertexIndex];

    VSOutput output;
    output.position = float4(vertex.position, 0.0, 1.0);
    output.color = vertex.color;

    return output;
}