# boden-gui
Cross platform retained-mode GUI framework.

[Click to watch the video:](https://www.youtube.com/watch?v=vQg3jZ5Dae8)
[![Watch the video](https://img.youtube.com/vi/vQg3jZ5Dae8/maxresdefault.jpg)](https://www.youtube.com/watch?v=vQg3jZ5Dae8)

- [Design Overview](#design-overview)
    - [Goals](#goals)
    - [Core Components](#core-components)


## Design Overview

The framework intentionally references Apples AppKit’s design patterns to reduces onboarding time and encourages best practices that have proven stable over decades of GUI development.

### Core Components

- window_t 
  
    window_t represents a top-level application window.

- view_controller_t

    view_controller_t manages the lifecycle and behavior of a group of views.

- view_t

    view_t is the fundamental building block of the UI.

- layer_t 

    layer_t represents a rendering surface associated with a view and corresponds to a separate GPU texture. It caches rendered content for efficient redraws and can be independently invalidated and updated.

### Supported platforms
- OSX (Metal)
- WWW (WebGL)
- Windows (TBD)
- Android (TBD)

### Build GUI library
```
git pull
git submodule update --init --recursive

mkdir build
cd build
cmake ..
make

ctest -V
```

### Build osx
Prerequisite: Xcode
Open Xcode project in `projects/osx` folder and click run.

### Build www
```
git pull;git submodule update --init
cd projects/www
source emsdk-install.sh
make

npm install 
npm run dev
```
