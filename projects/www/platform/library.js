addToLibrary({

    lib_begin_draw: function() {
        console.log("# begin_draw");     
    },

    lib_end_draw: function(
        commands_addr, commands_count,
        indices_addr, indices_count,
        vertices_addr, vertices_count
    ) {
        console.log("# end_draw");

        /*
         * struct command_t
         * uint32_t count;
         * uint32_t index_buffer_offset;
         * uint32_t vertex_buffer_offset;
         * boden::layout::rect_t clip_rect;
         * boden::asset::texture_id_t texture_id;
         */
        var commands = [];
        for (let i = 0; i < commands_count; i++) {
            let count = HEAP32[(commands_addr >> 2) + i * 8];           
            let index_buffer_offset = HEAP32[(commands_addr >> 2) + i * 8 + 1];  
            let vertex_buffer_offset = HEAP32[(commands_addr >> 2) + i * 8 + 2];
            let clip_rect_origin_x = HEAPF32[(commands_addr >> 2) + i * 8 + 3];
            let clip_rect_origin_y = HEAPF32[(commands_addr >> 2) + i * 8 + 4];
            let clip_rect_size_width = HEAPF32[(commands_addr >> 2) + i * 8 + 5];
            let clip_rect_size_height = HEAPF32[(commands_addr >> 2) + i * 8 + 6];
            let texture_id = HEAPU32[(commands_addr >> 2) + i * 8 + 7]; 

            commands.push([
                count,
                index_buffer_offset,
                vertex_buffer_offset,
                [{clip_rect_origin_x, clip_rect_origin_y}, {clip_rect_size_width,  clip_rect_size_height}],
                texture_id
            ]);
        }
        console.log(commands);

        var indices = [];
        for (let i = 0; i < indices_count; i++) {            
            indices.push(HEAPU32[(indices_addr >> 2) + i]);
        }
        console.log(indices);

        /*
         * strcut vertex_t
         * boden::layout::vec2_t position;
         * boden::layout::vec2_t uv;
         * uint32_t color;
        */
        var vertices = [];
        for (let i = 0; i < vertices_count; i++) {
            let position_x = HEAPF32[(vertices_addr >> 2) + i * 5];     
            let position_y = HEAPF32[(vertices_addr >> 2) + i * 5 + 1];   
            let uv_x = HEAPF32[(vertices_addr >> 2) + i * 5 + 2];    
            let uv_y = HEAPF32[(vertices_addr >> 2) + i * 5 + 3];   
            let color = HEAPU32[(vertices_addr >> 2) + i * 5 + 4];
            
            vertices.push([
                {position_x, position_y}, 
                {uv_x, uv_y},
                color
            ]);
        }
        console.log(vertices);

        Module.webgl_render(commands, indices, vertices);
    },

    lib_get_texture_id: function(_name) {
        const name = UTF8ToString(_name);
        const id = Module.webgl_get_texture_id(name);
        return id;
    }
    
});
    