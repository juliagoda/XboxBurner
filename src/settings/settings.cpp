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

#include "settings.h"

#include "listsettingswidget.h"

Settings::Settings(QSharedPointer<ListSettingsWidget> new_widgets_list,
                   const ApplicationInformations& applications_informations,
                   QObject* parent) :
    QSettings { applications_informations.getApplicationStorageFormat(),
                QSettings::UserScope,
                applications_informations.getOrganizationName(),
                applications_informations.getApplicationName().toLower(),
                parent }
    , widgets_list { new_widgets_list }
{}

void Settings::loadSettings()
{
    QListIterator<QSharedPointer<WidgetDataSaveStrategy>> widgets_list_iterator(*widgets_list.data()->getListWidgetStrategies());

    while (widgets_list_iterator.hasNext())
    {
        auto widget_pointer = widgets_list_iterator.next();
        auto widget = widget_pointer.data();
        widget->loadData(this);
    }
}

void Settings::saveSettings()
{
    QListIterator<QSharedPointer<WidgetDataSaveStrategy>> widgets_list_iterator(*widgets_list.data()->getListWidgetStrategies());

    while (widgets_list_iterator.hasNext())
    {
        auto widget_pointer = widgets_list_iterator.next();
        auto widget = widget_pointer.data();
        widget->saveData(this);
    }
}
