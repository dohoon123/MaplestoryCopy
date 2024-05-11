namespace Layout
{
    struct Vertex
    {
        float4 Position : POSITION;
        float2 TexCoord : TEXCOORD;//semantic name
    };
    
    struct Pixel
    {
        float4 Position : SV_POSITION;
        float2 TexCoord : TEXCOORD;
    };
    
    typedef float4 Color;
}