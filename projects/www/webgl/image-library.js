const textures = new Map();
const textureIds = new Map();

var lastTextureId = 0;

async function loadImageFromPath(gl, name, path) {
    const response = await fetch(path);
    const blob = await response.blob();
    const imageBitmap = await createImageBitmap(blob);
  
    const texture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.texImage2D(
        gl.TEXTURE_2D, 
        0, 
        gl.RGBA,
        gl.RGBA, 
        gl.UNSIGNED_BYTE, 
        imageBitmap
    );

    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);

    lastTextureId = lastTextureId + 1;

    textures.set(lastTextureId, texture);
    textureIds.set(name, lastTextureId);
}

function getTextureId(name) {
    return textureIds.get(name);
}

function getTexture(id) {
    return textures.get(id);
}


export { 
    loadImageFromPath,
    getTextureId,
    getTexture
};
