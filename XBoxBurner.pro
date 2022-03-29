TEMPLATE     = app
TARGET       = XBoxBurner
QT          += core gui widgets concurrent
DESTDIR      = bin
INCLUDEPATH  = src
OBJECTS_DIR  = build/o
UI_DIR       = build/ui
MOC_DIR      = build/moc
RCC_DIR      = build/rcc
CODECFORTR   = UTF-8
CODECFORSRC  = UTF-8

SOURCES   += src/main.cpp src/xboxburner.cpp \
    src/applicationinformations.cpp \
    src/mainwindowinitializator.cpp
HEADERS   += src/xboxburner.h src/global.h \
    src/applicationinformations.h \
    src/mainwindowinitializator.h
FORMS     += resources/ui/xboxburner.ui
RESOURCES += resources/xboxburner.qrc

linux-g++ {
  target.path    = /usr/bin
  icon.path     += /usr/share/pixmaps
  icon.files    += resources/images/XBoxBurner.png
  desktop.path  += /usr/share/applications
  desktop.files += resources/XBoxBurner.desktop
  INSTALLS      += target icon desktop
}

win32 {
  RC_FILE      = resources/win32icon.rc
  target.path  = "C:/Program Files/XBoxBurner"
  INSTALLS      += target
}

macx {
  ICON              += resources/images/XBoxBurner.icns
  QMAKE_INFO_PLIST  += resources/Info.plist
  CONFIG            += x86 ppc
  target.path        = /Applications
  INSTALLS          += target
}
