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

#include "mainwindowdatasavestrategy.h"

#include "settings.h"

MainWindowDataSaveStrategy::MainWindowDataSaveStrategy(QPointer<QMainWindow> new_main_window)
    : main_window { new_main_window }
{
}

void MainWindowDataSaveStrategy::loadData(QPointer<Settings> settings)
{
    QRect rect;
    rect.setX(settings.data()->value("MainWindow/x", QVariant(0)).toInt());
    rect.setY(settings.data()->value("MainWindow/y", QVariant(0)).toInt());
    rect.setWidth(settings.data()->value("MainWindow/width", QVariant(550)).toInt());
    rect.setHeight(settings.data()->value("MainWindow/height", QVariant(400)).toInt());

    main_window.data()->setGeometry(rect);
}

void MainWindowDataSaveStrategy::saveData(QPointer<Settings> settings)
{
    const QRect& main_window_geometry = main_window.data()->geometry();
    settings.data()->setValue("MainWindow/x", main_window_geometry.x());
    settings.data()->setValue("MainWindow/y", main_window_geometry.y());
    settings.data()->setValue("MainWindow/width", main_window_geometry.width());
    settings.data()->setValue("MainWindow/height", main_window_geometry.height());
}
