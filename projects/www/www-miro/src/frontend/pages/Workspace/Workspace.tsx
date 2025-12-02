import { FC, useEffect, useRef } from 'react';
import { 
  useDispatchDisplayEvents, 
  useDispatchKeyEvents, 
  useDispatchMouseEvents 
} from '@frontend/packages/worker';
import { useWindowSize } from '@frontend/packages/utils';

const Workspace: FC<{}> = () => {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const workerRef = useRef<Worker | null>(null);
  const { width, height } = useWindowSize();

  const { dispatchDisplaySize } = useDispatchDisplayEvents(workerRef);
  const { dispatchMouseDown, dispatchMouseMove, dispatchMouseUp } = useDispatchMouseEvents(workerRef);
  const { dispatchKeyDown, dispatchKeyUp } = useDispatchKeyEvents(workerRef);


  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    canvas.width = width * window.devicePixelRatio;
    canvas.height = height * window.devicePixelRatio;

    const offscreen = canvas.transferControlToOffscreen();

    const worker = new Worker(
      new URL('@frontend/workers/gui-backend.worker.js', import.meta.url),
      { type: 'module' }
    );

    worker.postMessage({ 
      type: 'init', 
      arg1: offscreen,
      arg2: window.devicePixelRatio,
      arg3: width,
      arg4: height
    }, [offscreen]);
    workerRef.current = worker;

    return () => {
      worker.terminate();
      workerRef.current = null;
    };
  }, []); 

  useEffect(() => {
    const handleDPRChange = () => {
      console.log("DPR changed:", window.devicePixelRatio);
    };
    console.log("######", window.devicePixelRatio);
    const mq = window.matchMedia(`(resolution: ${window.devicePixelRatio}dppx)`);
    mq.addEventListener("change", handleDPRChange);

    return () => mq.removeEventListener("change", handleDPRChange);
  }, []);

  useEffect(() => {
    dispatchDisplaySize(width, height);
  }, [width, height]);

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
