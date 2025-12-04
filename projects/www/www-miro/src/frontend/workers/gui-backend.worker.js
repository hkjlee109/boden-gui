import { mat4 } from 'gl-matrix';
import Module from './gui-backend.js';

let instance = null;
let backend = null;
let canvas = null;

let display_width = 0;
let display_height = 0;
let display_scale = 1;

self.onmessage = async (event) => {
    const { type, arg1, arg2, arg3, arg4 } = event.data;

    switch (type) {
    case 'init':
        canvas = arg1;
        display_scale = arg2;
        display_width = arg3;
        display_height = arg4;

        instance = await Module({
          locateFile: (filename) => {
            if (filename.endsWith('.wasm') || filename.endsWith('.data')) {
              return `/wasm/${filename}`;
            }
            return filename;
          }
        });

        instance.worker = self;
        instance.context = canvas.getContext('webgl');
        instance.mat4 = mat4
    
        if(instance.context === null) {
            console.log("Fatal: Unable to initialize WebGL. Your browser may not support it.");
           return;
        }

        canvas.width = display_width * display_scale;
        canvas.height = display_height * display_scale;

        backend = new instance.www_backend_t();
        backend.systemDisplayScaleChanged(display_scale);
        backend.systemDisplaySizeChanged(display_width, display_height);
        
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

    case 'scroll_wheel': 
        backend.scrollWheel(arg1, arg2);
        break;

    case 'system_display_scale_changed': 
        if(!backend) break;
        display_scale = arg1;
        canvas.width = display_width * display_scale;
        canvas.height = display_height * display_scale;
        backend.systemDisplayScaleChanged(arg1);
        break;
        
    case 'system_display_size_changed': 
        if(!backend) break;
        display_width = arg1;
        display_height = arg2;
        canvas.width = display_width * display_scale;
        canvas.height = display_height * display_scale;

        backend.systemDisplaySizeChanged(arg1, arg2);
        break;
    
    case 'system_text_input_committed': 
        backend.systemTextInputCommitted(arg1);
        break;
    }
};
