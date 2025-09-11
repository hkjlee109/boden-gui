import * as webgl from "./webgl/index.js"
import createModule from './public/boden-www.js';

let renderer;
let main_view_controller;
let image_library_ref;
let module = null;
let dragging = false;

const canvas = document.querySelector("#gl-canvas");
const worker = new Worker("./backend.js", { type: "module" });

createModule().then((_module) => {
    module = _module;
    module.webgl_render = webgl.render;
    module.webgl_get_texture_id = webgl.getTextureId;
    main();
});

canvas.addEventListener("mousedown", (event) => {
    const { x, y, button } = event;
    switch (event.button) {
    case 0:
        dragging = true;
        worker.postMessage({
            type: "left_mouse_down",
            x: x,
            y: y
        });
        break;
    }
});

canvas.addEventListener("mousemove", (event) => {
    const { x, y, button } = event;
    switch (event.button) {
    case 0:
        if (dragging) {
            worker.postMessage({
                type: "left_mouse_dragged",
                x: x,
                y: y
            });
        }
        break;
    }
});

canvas.addEventListener("mouseup", (event) => {
    const { x, y, button } = event;
    switch (event.button) {
    case 0:
        dragging = false;
        worker.postMessage({
            type: "left_mouse_up",
            x: x,
            y: y
        });
        break;
    }
});

async function main() {
    console.log('# Starting... ');

    const gl = canvas.getContext("webgl");

    if (gl === null) {
        alert("Unable to initialize WebGL. Your browser or machine may not support it.");
        return;
    }
    
    gl.enable(gl.BLEND);
    gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);

    await webgl.loadImageFromPath(gl, 'gearshape', './images/gearshape.png');

    webgl.setup(gl);

    let image_library = new module.webgl_image_library_t();
    image_library_ref = new module.image_library_ref_t(image_library);

    renderer = new module.webgl_renderer_t();

    worker.postMessage({
        type: "init"
    });

    // let rect = new module.rect_t(0, 0, 640, 480);
    // main_view_controller = module.make_main_view_controller(rect);
    // rect.delete();

    // main_view_controller.load_view();

    // let builder = new module.builder_t();
    // main_view_controller.draw(builder);

    // let context = new module.context_t();
    // context.batch = builder.get_batch();

    // console.log(context.batch);

    // renderer.render(context);

    // context.delete();
    // builder.delete();

    // if (renderer) {
    //     renderer.delete();
    //     renderer = null;
    // }

    // if (main_view_controller) {
    //     main_view_controller.delete();
    //     main_view_controller = null;
    // }
}

worker.onmessage = async (event) => {
    const { type, commands, indices, vertices } = event.data;

    console.log("[Main thread received]", event.data);

    if (type === "render") {
        const commands_view = new Uint8Array(commands);
        const indices_view = new Uint8Array(indices);
        const vertices_view = new Uint8Array(vertices);

        const context = new module.context_t();
        const batch = new module.batch_t();
        const commands_ptr = module._malloc(commands_view.length);
        module.HEAPU8.set(commands_view, commands_ptr);  
        batch.deserialize_commands(commands_ptr, commands_view.length);
        module._free(commands_ptr);

        const indices_ptr = module._malloc(indices_view.length);
        module.HEAPU8.set(indices_view, indices_ptr);
        batch.deserialize_indices(indices_ptr, indices_view.length);
        module._free(indices_ptr);

        const vertices_ptr = module._malloc(vertices_view.length);
        module.HEAPU8.set(vertices_view, vertices_ptr);
        batch.deserialize_vertices(vertices_ptr, vertices_view.length);
        module._free(vertices_ptr);

        context.batch = batch;
        renderer.render(context);
      
        batch.delete();
        context.delete();
    }
};