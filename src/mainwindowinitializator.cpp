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

#include "mainwindowinitializator.h"

#include "applicationinformations.h"
#include "factories/dvdrwtoolstooldependencyfactory.h"
#include "fileextensions.h"
#include "listsettingswidget.h"
#include "strategies/widgetdatasavestrategy.h"
#include "ui_xboxburner.h"

#include <QCompleter>
#include <QFileSystemModel>
#include <QFont>
#include <QFuture>
#include <QList>

MainWindowInitializator::MainWindowInitializator(const ApplicationInformations& new_applications_informations, QWidget* parent)
    : XBoxBurner { new_applications_informations, parent }
{
    settings = new Settings(createListOfSaveLoadStrategies(), new_applications_informations);
    fillPlainTextWithLogs(createListOfExternalDependencies(), new_applications_informations);
    initializeSettingsLoad();
    preparePathCompleters();
    showStatusBarMessage(tr("Ready."));
    prepareFontStyleForInformationLabel();
}

void MainWindowInitializator::initializeSettingsLoad()
{
    settings->loadSettings();
}

void MainWindowInitializator::initializeConnections()
{
    QObject::connect(&backup_future_watcher, &QFutureWatcher<bool>::finished, this, &MainWindowInitializator::logBackupCreation);
    QObject::connect(&image_future_watcher, &QFutureWatcher<QString>::finished, this, &MainWindowInitializator::calculateMD5HashOfImage);
    QObject::connect(&dvd_future_watcher, &QFutureWatcher<QString>::finished, this, &MainWindowInitializator::calculateMD5HashOfDVD);
    QObject::connect(this, &MainWindowInitializator::md5SumMaximum, this, &MainWindowInitializator::setMaximalPossibleMD5HashValue);
    QObject::connect(this, &MainWindowInitializator::md5SumProgress, this, &MainWindowInitializator::calculateMD5HashForProgressBar);
}

void MainWindowInitializator::prepareFontStyleForInformationLabel()
{
    QPointer<QLabel> informations_label { ui->label_info };
    QFont font { informations_label->font() };
    font.setBold(true);
    informations_label->setFont(font);
}

void MainWindowInitializator::preparePathCompleters()
{
    const QStringList file_extensions = QStringList() << FileExtensions::getISOName()
                                                      << FileExtensions::getIMGName()
                                                      << FileExtensions::getCDRName();
    const QStringList name_filters_for_cd_format { file_extensions };
    QPointer<QLineEdit> image_path_place { ui->lineedit_image_path };
    preparePathCompleter(image_path_place, name_filters_for_cd_format);

    QPointer<QLineEdit> burner_path_place { ui->lineedit_burner_path };
    preparePathCompleter(burner_path_place, QStringList());
}

void MainWindowInitializator::preparePathCompleter(QPointer<QLineEdit> const completer_path_place, const QStringList& name_filters)
{
    QPointer<QCompleter> path_completer { new QCompleter(this) };
    QPointer<QFileSystemModel> file_system_model { new QFileSystemModel(path_completer) };
    file_system_model->setNameFilters(name_filters);
    file_system_model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    path_completer->setModel(file_system_model);
    path_completer->setCaseSensitivity(Qt::CaseInsensitive);
    path_completer->setMaxVisibleItems(12);
    completer_path_place->setCompleter(path_completer);
}

void MainWindowInitializator::showMainWindow()
{
    if (!mainWindowShowed()) {
        show();
    }
}

bool MainWindowInitializator::mainWindowShowed()
{
    return isVisible();
}

void MainWindowInitializator::fillPlainTextWithLogs(const QList<QSharedPointer<DvdrwtoolsDependencyFactory>>& external_dependencies_list, const ApplicationInformations& new_applications_informations)
{
    ui->plain_text_edit_with_logs->appendPlainText(tr("(%1) XBoxBurner %2 started.").arg(QDateTime::currentDateTime().toString(), new_applications_informations.getApplicationVersion()));

    QListIterator<QSharedPointer<DvdrwtoolsDependencyFactory>> external_dependencies_list_iterator(external_dependencies_list);

    while (external_dependencies_list_iterator.hasNext()) {

        auto external_dependency = external_dependencies_list_iterator.next();
        ui->plain_text_edit_with_logs->appendPlainText(external_dependency.data()->showToolDetectionInformations().join("\n"));
    }
}

const QSharedPointer<ListSettingsWidget> MainWindowInitializator::createListOfSaveLoadStrategies()
{
    QSharedPointer<ListSettingsWidget> save_load_strategies { new ListSettingsWidget };
    save_load_strategies->addSettingStrategyCheckBox(ui->check_box_dao_mode);
    save_load_strategies->addSettingStrategyCheckBox(ui->check_box_dry_run);
    save_load_strategies->addSettingStrategyCheckBox(ui->check_box_dvd_compat);
    save_load_strategies->addSettingStrategyCheckBox(ui->check_box_backup_creation);
    save_load_strategies->addSettingStrategyCheckBox(ui->check_box_data_verification);
    save_load_strategies->addSettingStrategyLineEdit(ui->lineedit_burner_path);
    save_load_strategies->addSettingStrategyLineEdit(ui->lineedit_image_path);
    save_load_strategies->addSettingStrategyComboBox(ui->combo_box_dvd_format);
    save_load_strategies->addSettingStrategyToolBar(ui->toolBar);
    save_load_strategies->addSettingStrategyComboBox(ui->combo_box_dvd_format);
    save_load_strategies->addSettingStrategyMainWindow(this);

    return save_load_strategies;
}

const QList<QSharedPointer<DvdrwtoolsDependencyFactory>> MainWindowInitializator::createListOfExternalDependencies()
{
    QList<QSharedPointer<DvdrwtoolsDependencyFactory>> external_dependencies_list;
    QSharedPointer<DvdrwtoolsDependencyFactory> growisofs { new DvdrwtoolsToolDependencyFactory("growisofs") };
    external_dependencies_list.append(growisofs);
    QSharedPointer<DvdrwtoolsDependencyFactory> media_info { new DvdrwtoolsToolDependencyFactory("dvd+rw-mediainfo") };
    external_dependencies_list.append(media_info);

    return external_dependencies_list;
}
