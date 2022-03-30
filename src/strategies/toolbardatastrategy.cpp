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

#include "toolbardatastrategy.h"

#include "settings.h"

ToolBarDataStrategy::ToolBarDataStrategy(QPointer<QToolBar> tool_bar)
    : tool_bar { tool_bar }
{
}

void ToolBarDataStrategy::loadData(QPointer<Settings> settings)
{
    QVariant tool_button_style_variant = settings.data()->value("MainWindow/ToolButtonStyle", 2);
    tool_bar.data()->setToolButtonStyle(static_cast<Qt::ToolButtonStyle>(tool_button_style_variant.toInt()));
}

void ToolBarDataStrategy::saveData(QPointer<Settings> settings)
{
    settings.data()->setValue("MainWindow/ToolButtonStyle", tool_bar->toolButtonStyle());
}
