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

SOURCES += src/backup.cpp \
    src/burner/burnerprogressbarssetup.cpp \
    src/burner/burnerstartstage.cpp \
    src/burner/burningacceptancestage.cpp \
    src/burner/currentburningverificationstage.cpp \
    src/burner/pathsandspeedcontentverificationstage.cpp \
    src/burner/truncateimagestartstage.cpp \
    src/constants/applicationinformations.cpp \
    src/constants/boxmessages.cpp \
    src/constants/fileextensions.cpp \
    src/constants/messages.cpp \
    src/constants/regularexpressions.cpp \
    src/dependencies/dvdrwtoolsdependencyfactory.cpp \
    src/dependencies/dvdrwtoolstooldependencyfactory.cpp \
    src/dependencies/externaldependencyfactory.cpp \
    src/main.cpp \
    src/mainwindowinitializator.cpp \
    src/settings/checkboxdatasavestrategy.cpp \
    src/settings/comboboxdatasavestrategy.cpp \
    src/settings/lineeditdatasavestrategy.cpp \
    src/settings/listsettingswidget.cpp \
    src/settings/mainwindowdatasavestrategy.cpp \
    src/settings/toolbardatastrategy.cpp \
    src/settings/settings.cpp \
    src/verification/burnerpathvalidation.cpp \
    src/verification/cancelstate.cpp \
    src/verification/dvdverificationstate.cpp \
    src/verification/imageverificationstate.cpp \
    src/verification/startstate.cpp \
    src/verification/verification.cpp \
    src/verification/verificationstate.cpp \
    src/xboxburner.cpp

HEADERS   += src/backup.h \
    src/burner/burnerprogressbarssetup.h \
    src/burner/burnerstage.h \
    src/burner/burnerstartstage.h \
    src/burner/burnerwidgets.h \
    src/burner/burningacceptancestage.h \
    src/burner/currentburningverificationstage.h \
    src/burner/pathsandspeedcontentverificationstage.h \
    src/burner/truncateimagestartstage.h \
    src/constants/applicationinformations.h \
    src/constants/boxmessages.h \
    src/constants/fileextensions.h \
    src/constants/messages.h \
    src/constants/regularexpressions.h \
    src/dependencies/dvdrwtoolsdependencyfactory.h \
    src/dependencies/dvdrwtoolstooldependencyfactory.h \
    src/dependencies/externaldependencyfactory.h \
    src/mainwindowinitializator.h \
    src/settings/checkboxdatasavestrategy.h \
    src/settings/comboboxdatasavestrategy.h \
    src/settings/lineeditdatasavestrategy.h \
    src/settings/listsettingswidget.h \
    src/settings/mainwindowdatasavestrategy.h \
    src/settings/settings.h \
    src/settings/toolbardatastrategy.h \
    src/settings/widgetdatasavestrategy.h \
    src/verification/burnerpathvalidation.h \
    src/verification/cancelstate.h \
    src/verification/dvdverificationstate.h \
    src/verification/imageverificationstate.h \
    src/verification/startstate.h \
    src/verification/verification.h \
    src/verification/verificationstate.h \
    src/xboxburner.h

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
