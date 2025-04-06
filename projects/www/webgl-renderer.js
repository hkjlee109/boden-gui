const importObject = {
    env: {
        __memory_base: 0,
    }
};

WebAssembly.instantiateStreaming(fetch("./build/webgl-renderer.wasm"), importObject)
    .then(result => {
        const value = result.instance.exports.increment(1);
        console.log(value.toString());
    });
    