import { mat4 } from 'gl-matrix';
import Module from './gui-backend.js';

let instance = null;
let backend = null;

self.onmessage = async (event) => {
    const { type, arg1, arg2 } = event.data;

    switch (type) {
    case 'init':
        instance = await Module({
          locateFile: (filename) => {
            if (filename.endsWith('.wasm') || filename.endsWith('.data')) {
              return `/wasm/${filename}`;
            }
            return filename;
          }
        });

        instance.worker = self;
        instance.context = arg1.getContext('webgl');
        instance.mat4 = mat4
    
        if(instance.context === null) {
            console.log("Fatal: Unable to initialize WebGL. Your browser may not support it.");
           return;
        }

        backend = new instance.www_backend_t();

        console.log('[Worker] Init done.');

        backend.draw();
        break;

    case 'left_mouse_down':
        backend.mouseDown(arg1, arg2);
        break;

    case 'left_mouse_dragged':
        backend.mouseDragged(arg1, arg2);
        break;

    case 'left_mouse_up': 
        backend.mouseUp(arg1, arg2);
        break;

    case 'mouse_moved':
        backend.mouseMoved(arg1, arg2);
        break;

    case 'key_down': 
        backend.keyDown(arg1, 0, 0);
        break;

    case 'system_text_input_commit': 
        backend.commitTextInput(arg1);
        break;
    }
};
