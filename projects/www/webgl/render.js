import { getRenderProgram } from "./render-program.js"
import { getTexture } from "./image-library.js"

async function render(commands, indices, vertices) {
    console.log("# render");

    const canvas = document.querySelector("#gl-canvas");
    const gl = canvas.getContext("webgl");

    if(gl === null) {
        alert("Unable to initialize WebGL. Your browser or machine may not support it.");
        return;
    }

    gl.clearColor(0.45, 0.55, 0.6, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);

    var positions = [];
    var texCoords = [];
    var colors = [];

    for (let i = 0; i < vertices.length; i++) {
        positions = positions.concat(vertices[i][0].position_x, vertices[i][0].position_y);
        texCoords = texCoords.concat(vertices[i][1].uv_x, vertices[i][1].uv_y);

        const a = (vertices[i][2] >> 24) & 0xFF;
        const r = (vertices[i][2] >> 16) & 0xFF;
        const g = (vertices[i][2] >> 8)  & 0xFF;
        const b = vertices[i][2] & 0xFF;
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
    const right = gl.canvas.clientWidth;
    const bottom = gl.canvas.clientHeight;
    const top = 0;
    const near = -1;
    const far = 1;
    
    const projectionMatrix = mat4.create();
    mat4.ortho(projectionMatrix, left, right, bottom, top, near, far);

    const renderProgram = getRenderProgram();

    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
    gl.vertexAttribPointer(
        renderProgram.attribLocations.position, // index
        2,                                      // size
        gl.FLOAT,                               // type
        false,                                  // normalized
        0,                                      // stride
        0                                       // offset
    );
    gl.enableVertexAttribArray(renderProgram.attribLocations.position);

    gl.bindBuffer(gl.ARRAY_BUFFER, texCoordBuffer);
    gl.vertexAttribPointer(
        renderProgram.attribLocations.texCoord, // index
        2,                                      // size
        gl.FLOAT,                               // type
        false,                                  // normalized
        0,                                      // stride
        0                                       // offset
    );
    gl.enableVertexAttribArray(renderProgram.attribLocations.texCoord);

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
        for(let i = 0; i < commands.length; i++) {
            const offset = commands[i][1];
            const count = commands[i][0];
            const textureId = commands[i][4];
            const type = gl.UNSIGNED_SHORT;

            if(textureId) {
                const texture = getTexture(textureId);
                gl.activeTexture(gl.TEXTURE1);
                gl.bindTexture(gl.TEXTURE_2D, texture);
                gl.uniform1i(renderProgram.uniformLocations.texture, 1);
            } else {
                gl.uniform1i(renderProgram.uniformLocations.texture, 0);
            }

            gl.drawElements(gl.TRIANGLE_STRIP, count, type, offset*2);
        }
    }
}

export {
    render
};
