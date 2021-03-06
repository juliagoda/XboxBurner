/***************************************************************************
 *   Copyright (C) 2022 Jagoda "juliagoda" Górska                          *
 *   juliagoda.pl@protonmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, go to http://www.gnu.org             *
 ***************************************************************************/

#pragma once

#include "constants/applicationinformations.h"
#include "dependencies/dvdrwtoolsdependencyfactory.h"
#include "settings/widgetdatasavestrategy.h"
#include "xboxburner.h"

#include <QLineEdit>
#include <QPointer>
#include <QSharedPointer>
#include <QStringList>
#include <QWidget>

class ListSettingsWidget;

class MainWindowInitializator : public XBoxBurner
{
    Q_OBJECT

public:
    explicit MainWindowInitializator(const ApplicationInformations& new_applications_informations,
                                     QWidget* parent = nullptr);
    void showMainWindow();

private:
    void initializeSettingsLoad();
    void preparePathCompleter(QPointer<QLineEdit> const completer_path_place,
                              const QStringList& name_filters);
    void prepareFontStyleForInformationLabel();
    void preparePathCompleters();
    void fillPlainTextWithLogs(const QList<QSharedPointer<DvdrwtoolsDependencyFactory>>& external_dependencies_list,
                               const ApplicationInformations& new_applications_informations);
    bool mainWindowVisible();
    const QSharedPointer<ListSettingsWidget> createListOfSaveLoadStrategies();
    const QList<QSharedPointer<DvdrwtoolsDependencyFactory>> createListOfExternalDependencies();
};
