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

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QList>
#include <QMainWindow>
#include <QToolBar>

class WidgetDataSaveStrategy;

class ListSettingsWidget
{
public:
    ListSettingsWidget();
    void addSettingStrategyCheckBox(QCheckBox* checkbox);
    void addSettingStrategyLineEdit(QLineEdit* line_edit);
    void addSettingStrategyToolBar(QToolBar* tool_bar);
    void addSettingStrategyComboBox(QComboBox* combo_box);
    void addSettingStrategyMainWindow(QMainWindow* main_window);
    const QSharedPointer<QList<QSharedPointer<WidgetDataSaveStrategy>>> getListWidgetStrategies(); // QList cannot be inherited, so it's here wrapped

private:
    const QSharedPointer<QList<QSharedPointer<WidgetDataSaveStrategy>>> list_widget_strategies;
};
