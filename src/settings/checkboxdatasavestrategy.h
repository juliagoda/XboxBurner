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

#include "widgetdatasavestrategy.h"

#include <QCheckBox>
#include <QPointer>

class Settings;

class CheckBoxDataSaveStrategy : public WidgetDataSaveStrategy
{
public:
    CheckBoxDataSaveStrategy(QPointer<QCheckBox> new_checkbox);
    void loadData(QPointer<Settings> settings) override;
    void saveData(QPointer<Settings> settings) override;

private:
    QPointer<QCheckBox> checkbox;
};