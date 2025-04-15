import { setup as webgl_setup } from "./webgl/index.js"

import createModule from './build/boden-www.js';

createModule().then((Module) => {
    Module.webgl_setup = webgl_setup;

    main(Module);
});

let renderer;
let main_view_controller;

function main(module) {
    console.log('# Starting... ');

    const canvas = document.querySelector("#gl-canvas");
    const gl = canvas.getContext("webgl");

    if (gl === null) {
        alert("Unable to initialize WebGL. Your browser or machine may not support it.");
        return;
    }

    webgl_setup(gl);
    
    renderer = new module.webgl_renderer_t();

    let rect = new module.rect_t(0, 0, 640, 480);
    main_view_controller = new module.main_view_controller_t(rect);
    rect.delete();

    let context = new module.context_t();
    context.renderer = renderer;

    main_view_controller.draw(context);

    context.delete();

    if (renderer) {
        renderer.delete();
        renderer = null;
    }

    if (main_view_controller) {
        main_view_controller.delete();
        main_view_controller = null;
    }
}
