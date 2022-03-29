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
#include "ui_xboxburner.h"

#include <QCompleter>
#include <QFileSystemModel>
#include <QFont>
#include <QFuture>

MainWindowInitializator::MainWindowInitializator(QWidget* parent)
    : XBoxBurner { parent }
{
    checkTools();
    loadSettings();
    preparePathCompleters();
    showStatusBarMessage(tr("Ready."));
    prepareFontStyleForInformationLabel();
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
    const QStringList file_extensions = QStringList() << "*.iso"
                                                      << "*.img"
                                                      << "*.cdr";
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
    if (!mainWindowShowed())
        show();
}

bool MainWindowInitializator::mainWindowShowed()
{
    return isVisible();
}
