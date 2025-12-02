import { useCallback } from 'react';

export function useDispatchDisplayEvents(workerRef: React.RefObject<Worker | null>) {

    const dispatchDisplaySize = useCallback((width: number, height: number) => {
        const worker = workerRef.current;
        if(!worker) return;

        worker.postMessage({
            type: 'system_display_size_changed',
            arg1: width,
            arg2: height
        });
    }, []);

    return {
        dispatchDisplaySize
    };
}
