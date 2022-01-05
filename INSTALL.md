This document describes how you can compile XBoxBurner  

<br/>

## What you need

Make sure you get the following stuff:

* QT library - qt5 base (at least 5.15 highly recommended)
* dvd+rw-tools (on Linux - contains growisofs - must be accessible in PATH, and it is default)
* growisofs.exe (on Windows - must be in the same directory of XBoxBurner)

It is important that you compile AND INSTALL the above packages!

Further you will need GNU make and a working C++ compiler.  

<br/>

## Compiling XBoxBurner

```
cd XBoxBurner
mkdir build
qmake ..
make
make install (as root)
```

qmake uses the default version of Qt, however if you have parallel versions of Qt installed,
you can select them using qmake-qt5 or qmake6 instead of the default qmake  

For Windows you need the MinGW compiler tools: http://www.mingw.org  

<br/>

## Removing XBoxBurner

`sudo make uninstall`
