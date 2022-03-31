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

QMAKE_CXXFLAGS += -std=c++2a -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic

SOURCES   += src/main.cpp src/xboxburner.cpp \
    src/applicationinformations.cpp \
    src/factories/dvdrwtoolsdependencyfactory.cpp \
    src/factories/dvdrwtoolstooldependencyfactory.cpp \
    src/factories/externaldependencyfactory.cpp \
    src/strategies/checkboxdatasavestrategy.cpp \
    src/strategies/comboboxdatasavestrategy.cpp \
    src/fileextensions.cpp \
    src/strategies/lineeditdatasavestrategy.cpp \
    src/listsettingswidget.cpp \
    src/strategies/mainwindowdatasavestrategy.cpp \
    src/mainwindowinitializator.cpp \
    src/settings.cpp \
    src/strategies/toolbardatastrategy.cpp

HEADERS   += src/xboxburner.h \
    src/applicationinformations.h \
    src/factories/dvdrwtoolsdependencyfactory.h \
    src/factories/dvdrwtoolstooldependencyfactory.h \
    src/factories/externaldependencyfactory.h \
    src/strategies/checkboxdatasavestrategy.h \
    src/strategies/comboboxdatasavestrategy.h \
    src/fileextensions.h \
    src/strategies/lineeditdatasavestrategy.h \
    src/listsettingswidget.h \
    src/strategies/mainwindowdatasavestrategy.h \
    src/mainwindowinitializator.h \
    src/settings.h \
    src/strategies/toolbardatastrategy.h \
    src/strategies/widgetdatasavestrategy.h

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
