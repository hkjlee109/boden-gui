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

        const canvas = document.querySelector("#gl-canvas");
        const gl = canvas.getContext("webgl");
    
        if(gl === null) {
            alert("Unable to initialize WebGL. Your browser or machine may not support it.");
            return;
        }
    
        gl.clearColor(0.0, 0.0, 1.0, 1.0);
        gl.clear(gl.COLOR_BUFFER_BIT);
        
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
            let count = HEAP32[(commands_addr >> 2) + i * 10];           
            let index_buffer_offset = HEAP32[(commands_addr >> 2) + i * 10 + 1];  
            let vertex_buffer_offset = HEAP32[(commands_addr >> 2) + i * 10 + 2];
            let clip_rect_origin_x = HEAPF32[(commands_addr >> 2) + i * 10 + 3];
            let clip_rect_origin_y = HEAPF32[(commands_addr >> 2) + i * 10 + 4];
            let clip_rect_size_width = HEAPF32[(commands_addr >> 2) + i * 10 + 5];
            let clip_rect_size_height = HEAPF32[(commands_addr >> 2) + i * 10 + 6];
            let texture_id_low = HEAPU32[(commands_addr >> 2) + i * 10 + 7]; 
            let texture_id_high = HEAPU32[(commands_addr >> 2) + i * 10 + 8];

            commands.push([
                count,
                index_buffer_offset,
                vertex_buffer_offset,
                [{clip_rect_origin_x, clip_rect_origin_y}, {clip_rect_size_width,  clip_rect_size_height}],
                (BigInt(texture_id_high) << 32n) | BigInt(texture_id_low)
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
    }
    
});
    