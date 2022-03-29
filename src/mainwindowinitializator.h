#pragma once

#include "xboxburner.h"

#include <QLineEdit>
#include <QPointer>
#include <QStringList>
#include <QWidget>

class QLineEdit;

class MainWindowInitializator : public XBoxBurner {
    Q_OBJECT

public:
    explicit MainWindowInitializator(QWidget* parent = nullptr);
    void showMainWindow();

private:
    void initializeConnections();
    void preparePathCompleter(QPointer<QLineEdit> const completer_path_place,
        const QStringList& name_filters);
    void prepareFontStyleForInformationLabel();
    void preparePathCompleters();
    bool mainWindowShowed();
};
