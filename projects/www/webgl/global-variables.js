let gl = null;
let renderProgram = null;

function setGL(_gl) {
    gl = _gl;
}

function getGL() {
    return gl;
}

function setRenderProgram(_renderProgram) {
    renderProgram = _renderProgram;
}

function getRenderProgram() {
    return renderProgram;
}

export {
    setGL,
    getGL,
    setRenderProgram,
    getRenderProgram
};
