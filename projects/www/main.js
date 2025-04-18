import * as webgl from "./webgl/index.js"

import createModule from './build/boden-www.js';

createModule().then((Module) => {
    Module.webgl_render = webgl.render;
    
    main(Module);
});

let renderer;
let main_view_controller;
let image_library_ref;

function main(module) {
    console.log('# Starting... ');

    const canvas = document.querySelector("#gl-canvas");
    const gl = canvas.getContext("webgl");

    if (gl === null) {
        alert("Unable to initialize WebGL. Your browser or machine may not support it.");
        return;
    }

    webgl.setup(gl);

    let image_library = new module.webgl_image_library_t();
    image_library_ref = new module.image_library_ref_t(image_library);

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
