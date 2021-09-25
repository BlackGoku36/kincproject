// shade_vert_main

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct shade_vert_main_uniforms
{
    float4x4 M;
    float4x4 P;
    float4x4 V;
};

struct shade_vert_main_out
{
    float3 fragPos [[user(locn0)]];
    float3 normal [[user(locn1)]];
    float4 gl_Position [[position]];
};

struct shade_vert_main_in
{
    float3 nor [[attribute(0)]];
    float3 pos [[attribute(1)]];
};

vertex shade_vert_main_out shade_vert_main(shade_vert_main_in in [[stage_in]], constant shade_vert_main_uniforms& uniforms [[buffer(1)]])
{
    shade_vert_main_out out = {};
    out.fragPos = float3((uniforms.M * float4(in.pos, 1.0)).xyz);
    out.normal = in.nor;
    out.gl_Position = ((uniforms.P * uniforms.V) * uniforms.M) * float4(in.pos, 1.0);
    out.gl_Position.z = (out.gl_Position.z + out.gl_Position.w) * 0.5;       // Adjust clip-space for Metal
    return out;
}

