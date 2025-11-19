#include <metal_stdlib>
using namespace metal;

struct main_uniforms_t
{
    float4x4 projection_matrix;
};

struct main_vertex_in_t
{
    float2 position [[attribute(0)]];
    float2 tex_coords [[attribute(1)]];
    uchar4 color [[attribute(2)]];
};

struct main_vertex_out_t
{
    float4 position [[position]];
    float2 tex_coords;
    float4 color;
};

vertex main_vertex_out_t main_vertex(main_vertex_in_t in [[stage_in]],
                                     constant main_uniforms_t &uniforms [[buffer(1)]])
{
    main_vertex_out_t out;
    out.position = uniforms.projection_matrix * float4(in.position, 0, 1);
    out.tex_coords = in.tex_coords;
    out.color = float4(in.color) / float4(255.0);
    return out;
}

fragment half4 main_fragment(main_vertex_out_t in [[stage_in]],
                             texture2d<half, access::sample> texture [[texture(0)]])
{
    constexpr sampler nearest_sampler(coord::normalized,
                                      min_filter::nearest,
                                      mag_filter::nearest,
                                      mip_filter::nearest);

    half4 tex_color = texture.sample(nearest_sampler, in.tex_coords);
    return half4(in.color) * tex_color;
}
