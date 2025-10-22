import { useCallback } from 'react';

export function useDispatchKeyEvents(workerRef: React.RefObject<Worker | null>) {
    const dispatchKeyDown = useCallback((e: KeyboardEvent) => {
        const worker = workerRef.current;
        if(!worker) return;

        worker.postMessage({
            type: 'keydown',
            key: e.key,
            code: e.code,
            ctrlKey: e.ctrlKey,
            shiftKey: e.shiftKey,
            altKey: e.altKey,
            metaKey: e.metaKey,
        });
      }, []);

    const dispatchKeyUp = useCallback((e: KeyboardEvent) => {
        const worker = workerRef.current;
        if(!worker) return;

        worker.postMessage({
            type: 'keyup',
            key: e.key,
            code: e.code,
            ctrlKey: e.ctrlKey,
            shiftKey: e.shiftKey,
            altKey: e.altKey,
            metaKey: e.metaKey,
        });
    }, []);

    return {
        dispatchKeyDown,
        dispatchKeyUp,
    };
}
