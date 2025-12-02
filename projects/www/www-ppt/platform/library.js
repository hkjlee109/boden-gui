
addToLibrary({

    em_bake: function(
        gtid,
        x, 
        y, 
        width,
        height, 
        bytes,
        length,
        number_of_channels
    ) {
        const gl = Module.context;
        const texture = Module.gpuTextureMap.get(gtid);

        if(!texture) {
            console.error(`Debug: em_bake: Texture not found.`, gtid);
            return;
        }

        gl.bindTexture(gl.TEXTURE_2D, texture);

        const pixelBytes = new Uint8Array(Module.HEAPU8.buffer, bytes, length);

        gl.texSubImage2D(
            gl.TEXTURE_2D,
            0,
            x,
            y,
            width,
            height,
            gl.RGBA,
            gl.UNSIGNED_BYTE,
            pixelBytes
        );
    },

    em_setup: function() {
        const gl = Module.context;

        gl.enable(gl.BLEND);
        gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);

        const vertexSource = `
            attribute vec2 positionIn;
            attribute vec2 texCoordIn;
            attribute vec4 colorIn;

            uniform mat4 projectionMatrix;

            varying lowp vec2 texCoordOut;
            varying lowp vec4 colorOut;

            void main() {
                gl_Position = projectionMatrix * vec4(positionIn, 0.0, 1.0);
                texCoordOut = texCoordIn;
                colorOut = colorIn / 255.0;
            }
        `;

        const fragmentSource = `
            precision mediump float;
            uniform sampler2D texture;

            varying lowp vec2 texCoordOut;
            varying lowp vec4 colorOut;

            void main() {
                vec4 texColor = texture2D(texture, texCoordOut);
                gl_FragColor = colorOut * texColor;
            }
        `;

        const vertexShader = gl.createShader(gl.VERTEX_SHADER);
        gl.shaderSource(vertexShader, vertexSource);
        gl.compileShader(vertexShader);

        if(!gl.getShaderParameter(vertexShader, gl.COMPILE_STATUS)) {
            console.error(`Debug: An error occurred compiling the shaders: ${gl.getShaderInfoLog(vertexShader)}`);
            gl.deleteShader(vertexShader);
            return;
        }

        const fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
        gl.shaderSource(fragmentShader, fragmentSource);
        gl.compileShader(fragmentShader);

        if(!gl.getShaderParameter(fragmentShader, gl.COMPILE_STATUS)) {
            console.error(`Debug: An error occurred compiling the shaders: ${gl.getShaderInfoLog(fragmentShader)}`);
            gl.deleteShader(fragmentShader);
            return;
        }

        const program = gl.createProgram();
        gl.attachShader(program, vertexShader);
        gl.attachShader(program, fragmentShader);
        gl.linkProgram(program);

        if(!gl.getProgramParameter(program, gl.LINK_STATUS)) {
            console.error(`Fatal: Unable to initialize the shader program: ${gl.getProgramInfoLog(program)}`);
            return;
        }

        const renderInfomation = {
            program: program,
            attribLocations: {
                position: gl.getAttribLocation(program, "positionIn"),
                texCoord: gl.getAttribLocation(program, "texCoordIn"),
                color: gl.getAttribLocation(program, "colorIn"),
            },
            uniformLocations: {
                projectionMatrix: gl.getUniformLocation(program, "projectionMatrix"),
                texture: gl.getUniformLocation(program, "texture"),
            },
        };

        Module.renderInfomation = renderInfomation;

        const texture = gl.createTexture()
        gl.activeTexture(gl.TEXTURE0);
        gl.bindTexture(gl.TEXTURE_2D, texture);
        
        const whitePixel = new Uint8Array([255, 255, 255, 255]);
  
        gl.texImage2D(
            gl.TEXTURE_2D,    // target
            0,                // level
            gl.RGBA,          // internalFormat
            1,                // width
            1,                // height
            0,                // border
            gl.RGBA,          // format
            gl.UNSIGNED_BYTE, // type
            whitePixel        // data
        );
  
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);

        Module.gpuTextureMap = Module.gpuTextureMap || new Map();
        Module.gpuTextureIdCounter = Module.gpuTextureIdCounter || 1;

        Module.gpuTextureMap.set(0, texture);


        Module.positionBuffer = gl.createBuffer();
        Module.texCoordBuffer = gl.createBuffer();
        Module.colorBuffer = gl.createBuffer();
        Module.indexBuffer = gl.createBuffer();
        Module.sharedFramebuffer = gl.createFramebuffer();

        console.log("# setup completed.");
    },

    em_render: function(
        command_groups_addr, command_groups_count,
        indices_addr, indices_count,
        vertices_addr, vertices_count,
        display_width, display_height
    ) {
        /*
         * struct command_group_view_t
         * boden::graphic::texture_id_t tid
         * boden::layout::rect_t frame
         * boden::graphic::compositing_operation_t operation
         * const boden::draw::command_t* commands
         * size_t command_count
         */
        let command_groups = [];
        for(let i = 0; i < command_groups_count; i++) {
            let base_addr = (command_groups_addr >> 2) + i * 8;
            let tid = HEAP32[base_addr];   
            let x = HEAPF32[base_addr + 1];   
            let y = HEAPF32[base_addr + 2];
            let width = HEAPF32[base_addr + 3];  
            let height = HEAPF32[base_addr + 4];
            let operation = HEAP32[base_addr + 5];
            let commands_addr = HEAP32[base_addr + 6];
            let commands_count = HEAP32[base_addr + 7];

            command_groups.push([
                tid,
                [{x, y}, {width, height}],
                operation,
                commands_addr,
                commands_count
            ]);
        }

        let indices = [];
        for(let i = 0; i < indices_count; i++) {            
            indices.push(HEAPU32[(indices_addr >> 2) + i]);
        }

        /*
         * strcut vertex_t
         * boden::layout::vec2_t position;
         * boden::layout::vec2_t uv;
         * uint32_t color;
        */
        let vertices = [];
        for(let i = 0; i < vertices_count; i++) {
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

        console.log(`Debug: Rendering...`);

        const gl = Module.context;
        const mat4 = Module.mat4;
        const renderInfomation = Module.renderInfomation

        if(gl === null || mat4 === null || renderInfomation === null) {
            console.error(`Error: Unable to initialize WebGL. Your browser may not support it.`);
            return;
        }

        gl.useProgram(renderInfomation.program);

        let positions = [];
        let texCoords = [];
        let colors = [];

        for(let i = 0; i < vertices.length; i++) {
            positions = positions.concat(vertices[i][0].position_x, vertices[i][0].position_y);
            texCoords = texCoords.concat(vertices[i][1].uv_x, vertices[i][1].uv_y);

            const a = (vertices[i][2] >> 24) & 0xFF;
            const b = (vertices[i][2] >> 16) & 0xFF;
            const g = (vertices[i][2] >> 8)  & 0xFF;
            const r = vertices[i][2] & 0xFF;
            colors = colors.concat(r, g, b, a);
        }

        const projectionMatrix = mat4.create();

        const left = 0;
        const right = gl.canvas.width;
        const bottom = 0;
        const top = gl.canvas.height;
        const near = -1;
        const far = 1;

        mat4.ortho(projectionMatrix, left, right, bottom, top, near, far);

        gl.uniformMatrix4fv(
            renderInfomation.uniformLocations.projectionMatrix,
            false,
            projectionMatrix
        );

        {
            gl.bindBuffer(gl.ARRAY_BUFFER, Module.positionBuffer);
            gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);
            gl.vertexAttribPointer(
                renderInfomation.attribLocations.position, // index
                2,                                         // size
                gl.FLOAT,                                  // type
                false,                                     // normalized
                0,                                         // stride
                0                                          // offset
            );
            gl.enableVertexAttribArray(renderInfomation.attribLocations.position);

            gl.bindBuffer(gl.ARRAY_BUFFER, Module.texCoordBuffer);
            gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(texCoords), gl.STATIC_DRAW);
            gl.vertexAttribPointer(
                renderInfomation.attribLocations.texCoord, // index
                2,                                         // size
                gl.FLOAT,                                  // type
                false,                                     // normalized
                0,                                         // stride
                0                                          // offset
            );
            gl.enableVertexAttribArray(renderInfomation.attribLocations.texCoord);

            gl.bindBuffer(gl.ARRAY_BUFFER, Module.colorBuffer);
            gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);
            gl.vertexAttribPointer(
                renderInfomation.attribLocations.color, // index
                4,                                      // size
                gl.FLOAT,                               // type
                false,                                  // normalized
                0,                                      // stride
                0                                       // offset
            );
            gl.enableVertexAttribArray(renderInfomation.attribLocations.color);

            gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, Module.indexBuffer);
            gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices), gl.STATIC_DRAW);

            for(const group of command_groups) { 
                const dst_tid = group[0];
                const dst_gtid = Module._getGpuTextureId(dst_tid);
                const dst_texture = Module.gpuTextureMap.get(dst_gtid);
                const dst_frame_size = group[1][1];
                const operation = group[2];
                
                gl.bindFramebuffer(gl.FRAMEBUFFER, Module.sharedFramebuffer);
                gl.framebufferTexture2D(
                    gl.FRAMEBUFFER,
                    gl.COLOR_ATTACHMENT0,
                    gl.TEXTURE_2D,
                    dst_texture,
                    0
                );
                
                switch(operation) {
                    case 0: // clear
                        gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
                        gl.clearColor(0.0, 0.0, 0.0, 0.0);
                        gl.clear(gl.COLOR_BUFFER_BIT);
                        break;

                    case 1: // copy
                        gl.blendFunc(gl.ONE, gl.ONE_MINUS_SRC_ALPHA);
                        break;

                    case 2: // source_over
                        gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
                        break;

                    default:
                        break;
                }

                let commands = [];
                for(let i = 0; i < group[4]; i++) {
                    let base_addr = (group[3] >> 2) + i * 9;
                    let type = HEAP32[base_addr];  
                    let count = HEAP32[base_addr + 1];           
                    let index_buffer_offset = HEAP32[base_addr + 2];  
                    let vertex_buffer_offset = HEAP32[base_addr + 3];
                    let x = HEAPF32[base_addr + 4];
                    let y = HEAPF32[base_addr + 5];
                    let width = HEAPF32[base_addr + 6];
                    let height = HEAPF32[base_addr + 7];
                    let tid = HEAPU32[base_addr + 8]; 

                    commands.push([
                        type,
                        count,
                        index_buffer_offset,
                        vertex_buffer_offset,
                        [{x, y}, {width,  height}],
                        tid
                    ]);
                }

                for(let i = 0; i < commands.length; i++) {
                    const type = commands[i][0];
                    const count = commands[i][1];
                    const offset = commands[i][2];
                    const clip_rect_origin = commands[i][4][0];
                    const clip_rect_size = commands[i][4][1];
                    const tid = commands[i][5];

                    let draw_type;
                    switch(type) {
                        case 0:
                            draw_type = gl.TRIANGLE_STRIP;
                            break;
                        case 1:
                            draw_type = gl.LINES;
                            break;
                        default:
                            draw_type = gl.TRIANGLE_STRIP;
                            break;
                    }

                    gl.enable(gl.SCISSOR_TEST);
                    gl.scissor(
                        clip_rect_origin.x,
                        clip_rect_origin.y,
                        clip_rect_size.width,
                        clip_rect_size.height
                    );

                    if(tid) {
                        const gtid = Module._getGpuTextureId(tid); 
                        const texture = Module.gpuTextureMap.get(gtid);
                        gl.activeTexture(gl.TEXTURE1);
                        gl.bindTexture(gl.TEXTURE_2D, texture);
                        gl.uniform1i(renderInfomation.uniformLocations.texture, 1);
                    } else {
                        const texture = Module.gpuTextureMap.get(0);
                        gl.activeTexture(gl.TEXTURE0);
                        gl.bindTexture(gl.TEXTURE_2D, texture);
                        gl.uniform1i(renderInfomation.uniformLocations.texture, 0);
                    }
                    gl.drawElements(draw_type, count, gl.UNSIGNED_SHORT, offset * 2);

                    gl.disable(gl.SCISSOR_TEST);
                }

                gl.bindFramebuffer(gl.FRAMEBUFFER, null);
            }
        }

        if(!Module.rootTextureSize) {
            Module.rootTextureSize = {
                width: 0,
                height: 0
            };
        }

        if(gl.canvas.width !== Module.rootTextureSize.width || 
           gl.canvas.height !== Module.rootTextureSize.height) {
            if(Module.rootTexture) {
                gl.deleteTexture(Module.rootTexture);
            }

            Module.rootTexture = gl.createTexture();
            Module.rootTextureSize.width = gl.canvas.width;
            Module.rootTextureSize.height = gl.canvas.height;

            gl.bindTexture(gl.TEXTURE_2D, Module.rootTexture);

            gl.texImage2D(
                gl.TEXTURE_2D,
                0,
                gl.RGBA,
                gl.canvas.width,
                gl.canvas.height,
                0,
                gl.RGBA,
                gl.UNSIGNED_BYTE,
                null
            );

            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
            gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
        }

        {
            console.log("# blit textures on the root texture");
            gl.blendFunc(gl.ONE, gl.ONE_MINUS_SRC_ALPHA);
            gl.bindFramebuffer(gl.FRAMEBUFFER, Module.sharedFramebuffer);
            gl.framebufferTexture2D(
                gl.FRAMEBUFFER,
                gl.COLOR_ATTACHMENT0,
                gl.TEXTURE_2D,
                Module.rootTexture,
                0
            );

            gl.clearColor(0, 0, 0, 0);
            gl.clear(gl.COLOR_BUFFER_BIT);

            gl.viewport(0, 0, Module.rootTextureSize.width, Module.rootTextureSize.height);

            let current_tid = 0;
            for(const group of command_groups) { 
                const src_tid = group[0];
                if(current_tid == src_tid) {
                    continue;
                }

                let frame_origin = group[1][0];
                let frame_size = group[1][1];

                current_tid = src_tid;
                const src_gtid = Module._getGpuTextureId(src_tid);
                const src_texture = Module.gpuTextureMap.get(src_gtid);

                gl.activeTexture(gl.TEXTURE1);
                gl.bindTexture(gl.TEXTURE_2D, src_texture);
                gl.uniform1i(renderInfomation.uniformLocations.texture, 1);
                
                const quad = new Float32Array([
                    frame_origin.x,                    frame_origin.y,                
                    frame_origin.x + frame_size.width, frame_origin.y,               
                    frame_origin.x,                    frame_origin.y + frame_size.height, 
                    frame_origin.x + frame_size.width, frame_origin.y + frame_size.height,
                ]);
                gl.bindBuffer(gl.ARRAY_BUFFER, Module.positionBuffer);
                gl.bufferData(gl.ARRAY_BUFFER, quad, gl.STREAM_DRAW);
                gl.vertexAttribPointer(
                    renderInfomation.attribLocations.position, 2, gl.FLOAT, false, 0, 0
                );
                gl.enableVertexAttribArray(renderInfomation.attribLocations.position);

                const quadTexCoord = new Float32Array([
                    0, 0,
                    1, 0,
                    0, 1,
                    1, 1,
                ]);

                gl.bindBuffer(gl.ARRAY_BUFFER, Module.texCoordBuffer);
                gl.bufferData(gl.ARRAY_BUFFER, quadTexCoord, gl.STREAM_DRAW);
                gl.vertexAttribPointer(
                    renderInfomation.attribLocations.texCoord, 2, gl.FLOAT, false, 0, 0
                );
                gl.enableVertexAttribArray(renderInfomation.attribLocations.texCoord);

                const colors = new Float32Array([
                    255, 255, 255, 255,
                    255, 255, 255, 255,
                    255, 255, 255, 255,
                    255, 255, 255, 255,
                ]);
                gl.bindBuffer(gl.ARRAY_BUFFER, Module.colorBuffer);
                gl.bufferData(gl.ARRAY_BUFFER, colors, gl.STATIC_DRAW);
                gl.vertexAttribPointer(
                    renderInfomation.attribLocations.color, 4, gl.FLOAT, false, 0, 0
                );
                gl.enableVertexAttribArray(renderInfomation.attribLocations.color);

                gl.enable(gl.SCISSOR_TEST);
                gl.scissor(
                    frame_origin.x,
                    frame_origin.y,
                    frame_size.width,
                    frame_size.height
                );

                gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

                gl.disable(gl.SCISSOR_TEST);
            }
        }

        {
            console.log("# display on screen");
            gl.bindFramebuffer(gl.FRAMEBUFFER, null);
            gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);
            gl.clearColor(0, 0, 0, 0);
            gl.clear(gl.COLOR_BUFFER_BIT);

            gl.activeTexture(gl.TEXTURE2);
            gl.bindTexture(gl.TEXTURE_2D, Module.rootTexture);
            gl.uniform1i(renderInfomation.uniformLocations.texture, 2);

            const quad = new Float32Array([
                0,                0,                
                gl.canvas.width,  0,                
                0,                gl.canvas.height, 
                gl.canvas.width,  gl.canvas.height,
            ]);
            gl.bindBuffer(gl.ARRAY_BUFFER, Module.positionBuffer);
            gl.bufferData(gl.ARRAY_BUFFER, quad, gl.STREAM_DRAW);
            gl.vertexAttribPointer(
                renderInfomation.attribLocations.position, 2, gl.FLOAT, false, 0, 0
            );
            gl.enableVertexAttribArray(renderInfomation.attribLocations.position);

            const quadTexCoord = new Float32Array([
                0, 1,
                1, 1,
                0, 0,
                1, 0,
            ]);

            gl.bindBuffer(gl.ARRAY_BUFFER, Module.texCoordBuffer);
            gl.bufferData(gl.ARRAY_BUFFER, quadTexCoord, gl.STREAM_DRAW);
            gl.vertexAttribPointer(
                renderInfomation.attribLocations.texCoord, 2, gl.FLOAT, false, 0, 0
            );
            gl.enableVertexAttribArray(renderInfomation.attribLocations.texCoord);

            const colors = new Float32Array([
                255, 255, 255, 255,
                255, 255, 255, 255,
                255, 255, 255, 255,
                255, 255, 255, 255,
            ]);
            gl.bindBuffer(gl.ARRAY_BUFFER, Module.colorBuffer);
            gl.bufferData(gl.ARRAY_BUFFER, colors, gl.STATIC_DRAW);
            gl.vertexAttribPointer(
                renderInfomation.attribLocations.color, 4, gl.FLOAT, false, 0, 0
            );
            gl.enableVertexAttribArray(renderInfomation.attribLocations.color);

            gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
        }
    },

    em_create_gpu_texture: function(width, height) {
        const gl = Module.context;
        const texture = gl.createTexture();

        gl.bindTexture(gl.TEXTURE_2D, texture);
        gl.texImage2D(
            gl.TEXTURE_2D, 
            0, 
            gl.RGBA, 
            width, 
            height, 
            0,
            gl.RGBA, 
            gl.UNSIGNED_BYTE, 
            null
        );

        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);

        const gtid = Module.gpuTextureIdCounter++;
        Module.gpuTextureMap.set(gtid, texture);
        return gtid;
    },

    em_destroy_gpu_texture: function(handle) {
        const gl = Module.context;

        const texture = Module.gpuTextureMap.get(handle);

        if(!texture) {
            console.warn("em_destroy_gpu_texture: Tried to destroy unknown texture id: ", handle);
            return;
        }

        gl.deleteTexture(texture);
        Module.gpuTextureMap.delete(handle);
    },

    em_begin_text_input: function(
        text_,
        textLength,
        x,
        y, 
        width,
        height
    ) {
        const text = UTF8ToString(text_, textLength);

        let worker = Module.worker;
        worker.postMessage({
            type: 'begin_text_input',
            arg1: text,
            arg2: x,
            arg3: y,
            arg4: width,
            arg5: height
        });
        return;
    },

    em_end_text_input: function() {
        let worker = Module.worker;
        worker.postMessage({
            type: 'end_text_input'
        });
        return;
    }

});
