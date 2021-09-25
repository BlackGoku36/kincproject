// shade_frag_main

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct shade_frag_main_out
{
    float4 FragColor [[color(0)]];
};

fragment shade_frag_main_out shade_frag_main()
{
    shade_frag_main_out out = {};
    out.FragColor = float4(1.0, 0.0, 0.0, 1.0);
    return out;
}

