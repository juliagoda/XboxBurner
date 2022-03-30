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

#include "listsettingswidget.h"

#include "strategies/checkboxdatasavestrategy.h"
#include "strategies/comboboxdatasavestrategy.h"
#include "strategies/lineeditdatasavestrategy.h"
#include "strategies/mainwindowdatasavestrategy.h"
#include "strategies/toolbardatastrategy.h"
#include "strategies/widgetdatasavestrategy.h"

ListSettingsWidget::ListSettingsWidget()
    : list_widget_strategies { new QList<QSharedPointer<WidgetDataSaveStrategy>> }
{
}

void ListSettingsWidget::addSettingStrategyCheckBox(QCheckBox* checkbox)
{
    QPointer<QCheckBox> checkbox_smart_pointer = checkbox;
    QSharedPointer<WidgetDataSaveStrategy> strategy { new CheckBoxDataSaveStrategy(checkbox_smart_pointer) };
    list_widget_strategies->append(strategy);
}

void ListSettingsWidget::addSettingStrategyLineEdit(QLineEdit* line_edit)
{
    QPointer<QLineEdit> line_edit_smart_pointer = line_edit;
    QSharedPointer<WidgetDataSaveStrategy> strategy { new LineEditDataSaveStrategy(line_edit_smart_pointer) };
    list_widget_strategies->append(strategy);
}

void ListSettingsWidget::addSettingStrategyToolBar(QToolBar* tool_bar)
{
    QPointer<QToolBar> tool_bar_smart_pointer = tool_bar;
    QSharedPointer<WidgetDataSaveStrategy> strategy { new ToolBarDataStrategy(tool_bar_smart_pointer) };
    list_widget_strategies->append(strategy);
}

void ListSettingsWidget::addSettingStrategyComboBox(QComboBox* combo_box)
{
    QPointer<QComboBox> combo_box_smart_pointer = combo_box;
    QSharedPointer<WidgetDataSaveStrategy> strategy { new ComboBoxDataSaveStrategy(combo_box_smart_pointer) };
    list_widget_strategies->append(strategy);
}

void ListSettingsWidget::addSettingStrategyMainWindow(QMainWindow* main_window)
{
    QPointer<QMainWindow> main_window_smart_pointer = main_window;
    QSharedPointer<WidgetDataSaveStrategy> strategy { new MainWindowDataSaveStrategy(main_window_smart_pointer) };
    list_widget_strategies->append(strategy);
}

const QSharedPointer<QList<QSharedPointer<WidgetDataSaveStrategy>>> ListSettingsWidget::getListWidgetStrategies()
{
    return list_widget_strategies;
}
