import createModule from './public/boden-www.js';

let main_view_controller = null;
let image_id_lookup_table_ref = null;
let module = null;

self.onmessage = async (event) => {
    const { type, x, y, image_id_lookup_table_entries } = event.data;

    switch (type) {
    case 'init':
        module = await createModule();

        let image_id_lookup_table = new module.image_id_lookup_table_t();
        for (const [name, id] of image_id_lookup_table_entries) {
            image_id_lookup_table.insert(name, id);
        }
        image_id_lookup_table_ref = new module.image_id_lookup_table_ref_t(image_id_lookup_table);

        const rect = new module.rect_t(0, 0, 640, 480);
        main_view_controller = module.make_main_view_controller(rect);
        main_view_controller.load_view();
        rect.delete();

        console.log('[Worker] Init done.');

        render();
        break;

    case 'left_mouse_down':
        main_view_controller.mouse_down({
            type: module.event_type_t.left_mouse_down,
            location: { x: x, y: y }
        });
        render();
        break;

    case 'left_mouse_dragged':
        main_view_controller.mouse_dragged({
            type: module.event_type_t.left_mouse_dragged,
            location: { x: x, y: y }
        });
        render();
        break;

    case 'left_mouse_up': 
        main_view_controller.mouse_up({
            type: module.event_type_t.left_mouse_up,
            location: { x: x, y: y }
        });
        render();
        break;
    }
};

async function render() {
    try {
        const builder = new module.builder_t();
        main_view_controller.draw(builder);

        let batch = builder.get_batch();

        const commands_size = batch.get_commands_byte_size();
        const commands_ptr = module._malloc(commands_size);
        batch.serialize_commands(commands_ptr, commands_size);
        const commands_view = new Uint8Array(module.HEAPU8.buffer, commands_ptr, commands_size);
        const commands = new Uint8Array(commands_view); 

        const indices_size = batch.get_indices_byte_size();
        const indices_ptr = module._malloc(indices_size);
        batch.serialize_indices(indices_ptr, indices_size);
        const indices_view = new Uint8Array(module.HEAPU8.buffer, indices_ptr, indices_size);
        const indices = new Uint8Array(indices_view); 

        const vertices_size = batch.get_vertices_byte_size();
        const vertices_ptr = module._malloc(vertices_size);
        batch.serialize_vertices(vertices_ptr, vertices_size);
        const vertices_view = new Uint8Array(module.HEAPU8.buffer, vertices_ptr, vertices_size);
        const vertices = new Uint8Array(vertices_view); 

        postMessage({
            type: "render",
            commands,
            indices,
            vertices
        }, [
            commands.buffer,
            indices.buffer,
            vertices.buffer
        ]);

        module._free(commands_ptr);
        module._free(indices_ptr);
        module._free(vertices_ptr);
        builder.delete();
    } catch (err) {
        self.postMessage({ error: err.message || err });
    }
}