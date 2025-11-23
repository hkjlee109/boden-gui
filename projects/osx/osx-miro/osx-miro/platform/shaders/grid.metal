#include <metal_stdlib>
using namespace metal;

struct grid_vertex_out_t
{
    float4 position [[position]];
    float2 uv;
};

struct grid_uniforms_t
{
    float2 offset;
    float zoom;
    float2 screen_size;
};

constant float k_grid_major_size = 80.0;
constant float k_grid_minor_size = 20.0;

float dist_to_line(float v, float spacing)
{
    float m = fmod(v, spacing);
    return min(m, spacing - m);
}

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

fragment float4 grid_fragment(grid_vertex_out_t in [[stage_in]],
                              constant grid_uniforms_t &uniforms [[buffer(0)]])
{
    float2 screen_position = float2(in.uv.x, 1.0 - in.uv.y) * uniforms.screen_size;
    float2 world_position = screen_position / uniforms.zoom + uniforms.offset;
    
    float grid_major_size = k_grid_major_size;
    float grid_minor_size = k_grid_minor_size;
    
    float major_thickness = 1 / uniforms.zoom;
    float minor_thickness = 1 / uniforms.zoom;
    
    bool minor_visible = (uniforms.zoom > 0.5);

    float major_dist_x = dist_to_line(world_position.x, grid_major_size);
    float major_dist_y = dist_to_line(world_position.y, grid_major_size);

    bool on_major = (major_dist_x < major_thickness ||
                     major_dist_y < major_thickness);

    float minor_dist_x = dist_to_line(world_position.x, grid_minor_size);
    float minor_dist_y = dist_to_line(world_position.y, grid_minor_size);

    bool on_minor = false;
    if(minor_visible)
    {
        on_minor = (minor_dist_x < minor_thickness ||
                    minor_dist_y < minor_thickness);
    }

    float4 major_color = float4(0.92, 0.92, 0.92, 1.0);
    float k = 0.04 * (grid_minor_size * uniforms.zoom / grid_major_size);
    float4 minor_color = float4(0.96 - k, 0.96 - k, 0.96 - k, 1.0);
    
    if(on_major) return major_color;
    if(on_minor) return minor_color;

    return float4(0.0);
}
