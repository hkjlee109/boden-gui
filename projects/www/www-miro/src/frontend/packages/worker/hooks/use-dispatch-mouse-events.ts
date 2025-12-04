import { useRef, useCallback } from 'react';

export function useDispatchMouseEvents(workerRef: React.RefObject<Worker | null>) {
    const draggingRef = useRef(false);

    const dispatchMouseDown = useCallback((e: React.MouseEvent<HTMLCanvasElement>) => {
        const worker = workerRef.current;
        if(!worker) return;

        const rect = e.currentTarget.getBoundingClientRect();
        const x = e.clientX - rect.left;
        const y = e.clientY - rect.top;

        switch (e.button) {
        case 0:
            draggingRef.current = true;
            worker.postMessage({
                type: 'left_mouse_down',
                arg1: x,
                arg2: y
            });
            break;
        }
    }, []);

    const dispatchMouseMove = useCallback((e: React.MouseEvent<HTMLCanvasElement>) => {
        const worker = workerRef.current;
        if(!worker) return;

        const rect = e.currentTarget.getBoundingClientRect();
        const x = e.clientX - rect.left;
        const y = e.clientY - rect.top;

        switch (e.button) {
        case 0:
            if(draggingRef.current) {
                worker.postMessage({ type: 'left_mouse_dragged', arg1: x, arg2: y });
            } else {
                worker.postMessage({ type: 'mouse_moved', arg1: x, arg2: y });
            }
            break;
        }
    }, []);

    const dispatchMouseUp = useCallback((e: React.MouseEvent<HTMLCanvasElement>) => {
        const worker = workerRef.current;
        if(!worker) return;

        const rect = e.currentTarget.getBoundingClientRect();
        const x = e.clientX - rect.left;
        const y = e.clientY - rect.top;

        switch (e.button) {
        case 0:
            draggingRef.current = false;
            worker.postMessage({
                type: 'left_mouse_up',
                arg1: x,
                arg2: y
            });
            break;
        }
    }, []);

    const dispatchScrollWheel = useCallback((e: WheelEvent) => {
        const worker = workerRef.current;
        if(!worker) return;

        e.preventDefault(); 

        worker.postMessage({
            type: 'scroll_wheel',
            arg1: -e.deltaX,
            arg2: -e.deltaY
        });
    }, []);

    return {
        dispatchMouseDown,
        dispatchMouseMove,
        dispatchMouseUp,
        dispatchScrollWheel
    };
}
