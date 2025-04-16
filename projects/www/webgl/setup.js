import { setRenderProgram } from "./render-program.js"

function setup(gl) {
    const vsSource = `
        attribute vec4 vertexIn;
        attribute vec4 colorIn;

        uniform mat4 projectionMatrix;

        varying lowp vec4 colorOut;

        void main() {
            gl_Position = projectionMatrix * vertexIn;
            colorOut = colorIn;
        }
    `;

    const fsSource = `
        varying lowp vec4 colorOut;

        void main() {
            gl_FragColor = colorOut;
        }
    `;

    const vertexShader = loadShader(gl, gl.VERTEX_SHADER, vsSource);
    const fragmentShader = loadShader(gl, gl.FRAGMENT_SHADER, fsSource);

    const shaderProgram = gl.createProgram();
    gl.attachShader(shaderProgram, vertexShader);
    gl.attachShader(shaderProgram, fragmentShader);
    gl.linkProgram(shaderProgram);

    if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
        alert(
            `Unable to initialize the shader program: ${gl.getProgramInfoLog(
                shaderProgram
            )}`
        );
        return;
    }

    const renderProgram = {
        program: shaderProgram,
        attribLocations: {
            vertex: gl.getAttribLocation(shaderProgram, "vertexIn"),
            color: gl.getAttribLocation(shaderProgram, "colorIn"),
        },
        uniformLocations: {
            projectionMatrix: gl.getUniformLocation(shaderProgram, "projectionMatrix")
        },
    };

    setRenderProgram(renderProgram);
}

function loadShader(gl, type, source) {
    const shader = gl.createShader(type);
    gl.shaderSource(shader, source);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        alert(
            `An error occurred compiling the shaders: ${gl.getShaderInfoLog(shader)}`
        );
        gl.deleteShader(shader);
        return null;
    }

    return shader;
}

export {
    setup
};