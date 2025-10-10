import { defineConfig } from 'vite';
import path from 'path';

export default defineConfig({
  server: {
    open: true,
    fs: {
      allow: [
        path.resolve(__dirname),
        path.resolve(__dirname, 'public')
      ]
    }
  },
  optimizeDeps: {
    exclude: ['deps']
  }
});
