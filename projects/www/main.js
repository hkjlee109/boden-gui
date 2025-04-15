import { setup as webgl_setup } from "./webgl/index.js"

main();

function main() {
    const canvas = document.querySelector("#gl-canvas");
    const gl = canvas.getContext("webgl");

    if(gl === null) {
        alert("Unable to initialize WebGL. Your browser or machine may not support it.");
        return;
    }

    webgl_setup(gl);
}
