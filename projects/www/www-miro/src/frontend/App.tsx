import {
  createBrowserRouter,
  RouterProvider,
} from 'react-router-dom';

import { 
  Workspace,
} from '@frontend/pages';

import './App.css';

const router = createBrowserRouter([
  { path: "/", element: <Workspace /> },
], {
  future: {
    v7_relativeSplatPath: true,
    v7_fetcherPersist: true,
    v7_normalizeFormMethod: true,
    v7_partialHydration: true,
    v7_skipActionErrorRevalidation: true,
  },
});

function App() {
  return (
    <RouterProvider router={router} future={{ v7_startTransition: true }}/>
  )
}

export default App;
