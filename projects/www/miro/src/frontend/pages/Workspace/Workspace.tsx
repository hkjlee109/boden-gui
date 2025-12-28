import { FC, useEffect, useRef } from 'react';
import { 
  useDispatchDisplayEvents, 
  useDispatchKeyEvents, 
  useDispatchMouseEvents 
} from '@frontend/packages/worker';
import { useWindowSize } from '@frontend/packages/utils';
import { Pen, Sticky } from "@frontend/assets/images"; 

const Workspace: FC<{}> = () => {
  const canvasRef = useRef<HTMLCanvasElement | null>(null);
  const workerRef = useRef<Worker | null>(null);
  const { width, height } = useWindowSize();
  const cursorRef = useRef<string>("grab");
  const cursorOverrideRef = useRef<string | null>(null);

  const { dispatchDisplayScale, dispatchDisplaySize } = useDispatchDisplayEvents(workerRef);
  const { 
    dispatchMouseDown, 
    dispatchMouseMove, 
    dispatchMouseUp, 
    dispatchScrollWheel 
  } = useDispatchMouseEvents(workerRef);
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

    worker.onmessage = (event: MessageEvent) => {
      const { type, arg1, arg2, arg3, arg4, arg5 } = event.data;

      switch(type) {
        case 'set_cursor': 
          switch(arg1) {
            case 0: // none
              cursorRef.current = "default";
              break;
            case 1: // arrow
              cursorRef.current = "default";
              break;
            case 2: // pointing hand
              cursorRef.current = "pointer";
              break;
            case 3: // open hand
              cursorRef.current = "grab";
              break;
            case 4: // sticky
              cursorRef.current = `url(${Sticky}) 12 12, auto`;
              break;
            case 5: // pen
              cursorRef.current = `url(${Pen}) 12 12, auto`;
              break;
            default:
              cursorRef.current = "default";
              break;
          }
          document.body.style.cursor = cursorOverrideRef.current ?? cursorRef.current;
          break;
        
        case 'set_cursor_override': 
          switch(arg1) {
            case 0: // none
              cursorOverrideRef.current = null;
              break;
            case 2: // pointing hand
              cursorOverrideRef.current = "pointer";
              break;
            default:
              cursorOverrideRef.current = null;
              break;
          }
          document.body.style.cursor = cursorOverrideRef.current ?? cursorRef.current;
          break;
      }
    };

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
    document.body.style.cursor = cursorOverrideRef.current ?? cursorRef.current;
  }, []);

  useEffect(() => {
    const mq = window.matchMedia(`(resolution: ${window.devicePixelRatio}dppx)`);
    mq.addEventListener("change", dispatchDisplayScale);
    return () => mq.removeEventListener("change", dispatchDisplayScale);
  }, []);

  useEffect(() => {
    const canvas = canvasRef.current!;
    canvas.addEventListener("wheel", dispatchScrollWheel, { passive: false });

    return () => {
      canvas.removeEventListener("wheel", dispatchScrollWheel);
    };
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
