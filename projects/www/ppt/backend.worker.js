import { mat4 } from 'gl-matrix';
import createModule from './public/boden-www.js';

let Module = null;
let backend = null;

self.onmessage = async (event) => {
    const { type, arg1, arg2 } = event.data;

    switch (type) {
    case 'init':
        Module = await createModule();
        Module.worker = self;
        Module.context = arg1.getContext('webgl');
        Module.mat4 = mat4

        if(Module.context === null) {
            console.log("Fatal: Unable to initialize WebGL. Your browser may not support it.");
            return;
        }
    
        backend = new Module.www_backend_t();

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

    case 'system_display_scale_changed': 
        backend.systemDisplayScaleChanged(arg1);
        break;
        
    case 'system_display_size_changed': 
        backend.systemDisplaySizeChanged(arg1, arg2);
        break;
    
    case 'system_text_input_committed': 
        backend.systemTextInputCommitted(arg1);
        break;
    }
};
