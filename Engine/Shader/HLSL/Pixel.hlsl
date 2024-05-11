#include "Layout.hlsli"

namespace Shader
{
    const Texture2D Resource : register(T0);
    
    Layout::Color Pixel(const Layout::Pixel Input) : SV_Target
    {
        Layout::Color Output =
        {
            Resource.Load(int3(Input.TexCoord.x, Input.TexCoord.y, 0))
        };
        //if (Output.a == 0.0f) discard;
        return Output;
    }

}