import { mapKeyCode } from './key-code.js';

let dragging = false;

const canvas = document.querySelector('#canvas');
const textInput = document.querySelector('#textInput');
const worker = new Worker('./backend.worker.js', { type: 'module' });

canvas.addEventListener('mousedown', (event) => {
    const x = event.offsetX;
    const y = event.offsetY;

    switch (event.button) {
    case 0:
        dragging = true;
        worker.postMessage({
            type: 'left_mouse_down',
            arg1: x,
            arg2: y
        });
        break;
    }
});

canvas.addEventListener('mousemove', (event) => {
    const x = event.offsetX;
    const y = event.offsetY;

    switch (event.button) {
    case 0:
        if (dragging) {
            worker.postMessage({
                type: 'left_mouse_dragged',
                arg1: x,
                arg2: y
            });
        } else {
            worker.postMessage({
                type: 'mouse_moved',
                arg1: x,
                arg2: y
            });
        }
        break;
    }
});

canvas.addEventListener('mouseup', (event) => {
    const x = event.offsetX;
    const y = event.offsetY;

    switch (event.button) {
    case 0:
        dragging = false;
        worker.postMessage({
            type: 'left_mouse_up',
            arg1: x,
            arg2: y
        });
        break;
    }
});

worker.onmessage = function(event) {
    const { type, arg1, arg2, arg3, arg4, arg5 } = event.data;

    switch(type) {
    case 'begin_text_input':
        const text = arg1;
        const canvasRect = canvas.getBoundingClientRect();
        const x = canvasRect.left + arg2;
        const y = canvasRect.top + arg3;
        const width = arg4;
        const height = arg5;

        textInput.value = text;
        textInput.style.display = 'block';
        textInput.style.left = `${x}px`;
        textInput.style.top = `${y}px`;
        textInput.style.width = `${width}px`;
        textInput.style.height = `${height}px`;
        textInput.focus();
        break;

    case 'end_text_input':
        worker.postMessage({
            type: 'system_text_input_commit',
            arg1: textInput.value
        });
        textInput.value = '';
        textInput.style.display = 'none';
        break;

   default:
        break;
    }
}

textInput.addEventListener('keydown', (event) => {
    switch(event.key) {
    case 'Escape':
        console.log('Escape key pressed!');
        textInput.style.display = 'none';
        worker.postMessage({
            type: 'system_text_input_commit',
            arg1: textInput.value
        });
        break;
    }
});

window.addEventListener('keydown', function(event) {
    worker.postMessage({
        type: 'key_down',
        arg1: mapKeyCode(event.code)
    });
});

async function main() {
    console.log('# Starting... ');

    const offscreen = canvas.transferControlToOffscreen();

    worker.postMessage({
        type: "init",
        arg1: offscreen
    }, [offscreen]);
}

main();
