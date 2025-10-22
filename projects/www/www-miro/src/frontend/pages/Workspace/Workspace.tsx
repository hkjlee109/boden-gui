import { FC, useEffect, useRef } from 'react';
import { useDispatchKeyEvents, useDispatchMouseEvents } from '@frontend/packages/worker';
import { useWindowSize } from '@frontend/packages/utils';

const Workspace: FC<{}> = () => {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const workerRef = useRef<Worker | null>(null);
  const { width, height } = useWindowSize();

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    canvas.width = width;
    canvas.height = height;

    const offscreen = canvas.transferControlToOffscreen();

    const worker = new Worker(
      new URL('@frontend/workers/gui-backend.worker.js', import.meta.url),
      { type: 'module' }
    );

    worker.postMessage({ type: 'init', arg1: offscreen }, [offscreen]);
    workerRef.current = worker;

    return () => {
      worker.terminate();
      workerRef.current = null;
    };
  }, []); 

  useEffect(() => {
    const canvas = canvasRef.current;
    if(!canvas) { return; }
  }, [width, height]);

  const { dispatchMouseDown, dispatchMouseMove, dispatchMouseUp } = useDispatchMouseEvents(workerRef);
  const { dispatchKeyDown, dispatchKeyUp } = useDispatchKeyEvents(workerRef);

  useEffect(() => {
    window.addEventListener('keydown', dispatchKeyDown);
    window.addEventListener('keyup', dispatchKeyUp);
    return () => {
      window.removeEventListener('keydown', dispatchKeyDown);
      window.removeEventListener('keyup', dispatchKeyUp);
    };
  }, [dispatchKeyDown, dispatchKeyUp]);

  return (
    <div className="w-screen h-screen bg-[#f2f2f2]">
      <canvas
        id="canvas"
        ref={canvasRef}
        className="block w-full h-full"
        onMouseDown={dispatchMouseDown}
        onMouseMove={dispatchMouseMove}
        onMouseUp={dispatchMouseUp}
      />
    </div>
  );
}

export { Workspace };
