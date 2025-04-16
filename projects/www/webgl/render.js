import { getRenderProgram } from "./render-program.js"

function render(gl, commands, indices, vertices) {
    console.log("# render");
    
    const renderProgram = getRenderProgram();

    var vertices_only = [];
    var colors_only = [];
    for (let i = 0; i < vertices.length; i++) {
        vertices_only = vertices_only.concat(vertices[i][0].position_x, vertices[i][0].position_y, 0);

        const a = (vertices[i][2] >> 24) & 0xFF;
        const r = (vertices[i][2] >> 16) & 0xFF;
        const g = (vertices[i][2] >> 8)  & 0xFF;
        const b = vertices[i][2] & 0xFF;
        colors_only = colors_only.concat(r, g, b, a);
    }

    const vertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);

    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices_only), gl.STATIC_DRAW);

    const colorBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors_only), gl.STATIC_DRAW);

    const indexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
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
        for (let i = 0; i < commands.length; i++) {
            const offset = commands[i][1];
            const count = commands[i][0];
            const type = gl.UNSIGNED_SHORT;
            gl.drawElements(gl.TRIANGLE_STRIP, count, type, offset);
        }
    }
}

export {
    render
};