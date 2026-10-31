
cbuffer CameraBuffer : register(b0)
{
    row_major float4x4 view;
    row_major float4x4 projection;
};

struct VertexInput
{
    float3 position : POSITION;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
};

VertexOutput VSMain(VertexInput input)
{
    VertexOutput output;

    float4 world_position = float4(input.position, 1.0f);

    output.position = mul(
        mul(world_position, view),
        projection
    );

    return output;
}
