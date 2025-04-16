import { getRenderProgram } from "./render-program.js"

function render(gl, commands_ig, indices_ig, vertices_ig) {
    console.log("# render");
    
    const renderProgram = getRenderProgram();

    const vertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
    const vertices = [
        101, 100,  0,  
        101, 150,  0,
         99, 100,  0,
         99, 150,  0,
    ];

    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);

    const colorBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
    const colors = [
        1.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 0.0, 1.0
    ];
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);

    const indexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
    const indices = [
        0, 1, 2, 
        0, 2, 3
    ];
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices), gl.STATIC_DRAW);

    gl.clearColor(0.45, 0.5, 0.6, 1.0);
    gl.clearDepth(1.0);

    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    const left = 0;
    const right = gl.canvas.clientWidth;
    const bottom = gl.canvas.clientHeight;
    const top = 0;
    const near = -1;
    const far = 1;
    
    const projectionMatrix = mat4.create();
    mat4.ortho(projectionMatrix, left, right, bottom, top, near, far);

    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
    gl.vertexAttribPointer(
        renderProgram.attribLocations.vertex, // index
        3,                                    // size
        gl.FLOAT,                             // type
        false,                                // normalized
        0,                                    // stride
        0                                     // offset
    );
    gl.enableVertexAttribArray(renderProgram.attribLocations.vertex);

    gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
    gl.vertexAttribPointer(
        renderProgram.attribLocations.color, // index
        4,                                   // size
        gl.FLOAT,                            // type
        false,                               // normalized
        0,                                   // stride
        0                                    // offset
    );
    gl.enableVertexAttribArray(renderProgram.attribLocations.color);

    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);

    gl.useProgram(renderProgram.program);

    gl.uniformMatrix4fv(
        renderProgram.uniformLocations.projectionMatrix,
        false,
        projectionMatrix
    );

    {
        const offset = 0;
        const vertexCount = 6;
        const type = gl.UNSIGNED_SHORT;
        gl.drawElements(gl.TRIANGLE_STRIP, vertexCount, type, offset);
    }
}

export {
    render
};