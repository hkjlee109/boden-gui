#include <metal_stdlib>
using namespace metal;

struct grid_vertex_out_t
{
    float4 position [[position]];
    float2 uv;
};

vertex grid_vertex_out_t grid_vertex(uint vertex_id [[vertex_id]])
{
    float2 positions[6] =
    {
        float2(-1.0, -1.0),
        float2( 1.0, -1.0),
        float2(-1.0,  1.0),
        float2(-1.0,  1.0),
        float2( 1.0, -1.0),
        float2( 1.0,  1.0)
    };


    grid_vertex_out_t out;
    out.position = float4(positions[vertex_id], 0.0, 1.0);
    out.uv = (positions[vertex_id] + 1.0) * 0.5;

    return out;
}

struct grid_uniforms_t
{
    float2 offset;
    float zoom;
    float major_spacing;
    float minor_spacing;
    float2 screen_size;
};

fragment float4 grid_fragment(grid_vertex_out_t in [[stage_in]],
                              constant grid_uniforms_t &uniforms [[buffer(0)]])
{
    float2 screen_position = in.uv * uniforms.screen_size;
    float2 world_position = screen_position / uniforms.zoom + uniforms.offset;

    float2 major_cell = fmod(world_position, uniforms.major_spacing);
    float2 minor_cell = fmod(world_position, uniforms.minor_spacing);

    float major_thickness = 1.0 / uniforms.zoom;
    float minor_thickness = 1.0 / uniforms.zoom;

    bool on_major = (fabs(major_cell.x) < major_thickness) ||
                    (fabs(major_cell.y) < major_thickness);

    bool minor_visible = (uniforms.zoom > 0.75);

    bool on_minor = false;
    if(minor_visible)
    {
        on_minor = (fabs(minor_cell.x) < minor_thickness) ||
                   (fabs(minor_cell.y) < minor_thickness);
    }

    float4 major_color = float4(0.88, 0.88, 0.88, 1.0);
    float4 minor_color = float4(0.92, 0.92, 0.92, 1.0);

    if(on_major) return major_color;
    if(on_minor) return minor_color;

    return float4(0.0);
}
