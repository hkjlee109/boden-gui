
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
            console.error(`Debug: An error occurred compiling the shaders: ${gl.getShaderInfoLog(shader)}`);
            gl.deleteShader(vertexShader);
            return;
        }

        const fragmentShader = gl.createShader(gl.FRAGMENT_SHADER);
        gl.shaderSource(fragmentShader, fragmentSource);
        gl.compileShader(fragmentShader);

        if(!gl.getShaderParameter(fragmentShader, gl.COMPILE_STATUS)) {
            console.error(`Debug: An error occurred compiling the shaders: ${gl.getShaderInfoLog(shader)}`);
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
    },

    em_render: function(
        commands_addr, commands_count,
        indices_addr, indices_count,
        vertices_addr, vertices_count
    ) {
        /*
         * struct command_t
         * uint32_t count;
         * uint32_t index_buffer_offset;
         * uint32_t vertex_buffer_offset;
         * boden::layout::rect_t clip_rect;
         * boden::gpu::texture_id_t texture_id;
         */
        let commands = [];
        for(let i = 0; i < commands_count; i++) {
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

        gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);
        gl.clearColor(0.95, 0.95, 0.95, 1.0);
        gl.clear(gl.COLOR_BUFFER_BIT);

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

        const positionBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

        const texCoordBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, texCoordBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(texCoords), gl.STATIC_DRAW);

        const colorBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);

        const indexBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
        gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices), gl.STATIC_DRAW);

        const left = 0;
        const right = gl.canvas.width;
        const bottom = gl.canvas.height;
        const top = 0;
        const near = -1;
        const far = 1;
    
        const projectionMatrix = mat4.create();
        mat4.ortho(projectionMatrix, left, right, bottom, top, near, far);

        gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
        gl.vertexAttribPointer(
            renderInfomation.attribLocations.position, // index
            2,                                         // size
            gl.FLOAT,                                  // type
            false,                                     // normalized
            0,                                         // stride
            0                                          // offset
        );
        gl.enableVertexAttribArray(renderInfomation.attribLocations.position);

        gl.bindBuffer(gl.ARRAY_BUFFER, texCoordBuffer);
        gl.vertexAttribPointer(
            renderInfomation.attribLocations.texCoord, // index
            2,                                         // size
            gl.FLOAT,                                  // type
            false,                                     // normalized
            0,                                         // stride
            0                                          // offset
        );
        gl.enableVertexAttribArray(renderInfomation.attribLocations.texCoord);

        gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
        gl.vertexAttribPointer(
            renderInfomation.attribLocations.color, // index
            4,                                      // size
            gl.FLOAT,                               // type
            false,                                  // normalized
            0,                                      // stride
            0                                       // offset
        );
        gl.enableVertexAttribArray(renderInfomation.attribLocations.color);

        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);

        gl.uniformMatrix4fv(
            renderInfomation.uniformLocations.projectionMatrix,
            false,
            projectionMatrix
        );

        {
            for(let i = 0; i < commands.length; i++) {
                const offset = commands[i][1];
                const count = commands[i][0];
                const tid = commands[i][4];
                const type = gl.UNSIGNED_SHORT;

                if(tid) {
                    const gtid = Module._cpp_get_gpu_texture_id(tid); 
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
                gl.drawElements(gl.TRIANGLE_STRIP, count, type, offset*2);
            }
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
    