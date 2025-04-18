let defaultTexture = null;

function init(gl) {
    if(!gl) {
        console.error("WebGL not supported");
    }

    defaultTexture = gl.createTexture();
    gl.activeTexture(gl.TEXTURE0);
    gl.bindTexture(gl.TEXTURE_2D, defaultTexture);
  
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
}

function getDefaultTexture() {
    return defaultTexture;
}

export { 
    init,
    getDefaultTexture 
};
