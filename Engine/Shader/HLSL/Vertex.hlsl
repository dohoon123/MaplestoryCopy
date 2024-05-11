#include "Layout.hlsli"

namespace Shader
{
    namespace Resource
    {
        cbuffer Transform : register(B0)
        {
            matrix Former;
        }
        cbuffer Transform : register(B1)
        {
            matrix Latter;
        }
        

    }
    Layout::Pixel Vertex(const Layout::Vertex Input)
    {
        Layout::Pixel Output =
        {
            Input.Position,
            Input.TexCoord
        };
        
        Output.Position = mul(Output.Position, Resource::Former);
        Output.Position = mul(Output.Position, Resource::Latter);
        
        return Output;

    }

}