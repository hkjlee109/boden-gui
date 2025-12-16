# boden-gui
Cross platform retained-mode GUI framework.

[Click to watch the video:](https://www.youtube.com/watch?v=vQg3jZ5Dae8)
[![Watch the video](https://img.youtube.com/vi/vQg3jZ5Dae8/maxresdefault.jpg)](https://www.youtube.com/watch?v=vQg3jZ5Dae8)

[Click to watch the video:](https://www.youtube.com/watch?v=2Slia4A-cnk)
[![Watch the video](https://img.youtube.com/vi/2Slia4A-cnk/maxresdefault.jpg)](https://www.youtube.com/watch?v=2Slia4A-cnk)

- [Design Overview](#design-overview)
    - [Core Components](#core-components)
- [Supported Platforms](#supported-platforms)
- [Build](#build)
    - [Build OSX Project](#build-osx-project)
    - [Build WWW Project](#build-www-project)

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

## Supported Platforms
- OSX (Metal)
- WWW (WebGL)
- Windows (TBD)
- Android (TBD)

## Build
```
git pull
git submodule update --init --recursive

mkdir build
cd build
cmake ..
make

ctest -V
```

### Build OSX Project
Prerequisite: Xcode
Open Xcode project in `projects/osx` folder and click run.

### Build WWW Project
```
git pull;git submodule update --init
cd projects/www
source emsdk-install.sh
make

npm install 
npm run dev
```
