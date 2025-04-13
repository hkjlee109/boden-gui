# boden-gui
Cross platform 2D graphic library.

### Supported platforms
- osx (metal)
- web (webgl)
- windows (TBD)
- android (TBD)

### Build GUI library
```
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
Prerequisite: VSCode with "Live Server" extension installed.
```
git pull;git submodule update --init
cd projects/www
source emsdk-install.sh
make
```
Open VSCode workspace (`boden-www.code-workspace`) and Open `index.html` with Live Server.

