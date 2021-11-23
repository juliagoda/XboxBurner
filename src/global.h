/***************************************************************************
 *   Copyright (C) 2011 - 2012 Kai Heitkamp                                *
 *   dynup@ymail.com | dynup.de.vu                                         *
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

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QSettings>

const QString AppName("XBoxBurner");
const QString AppOrg("dynup");

#ifdef Q_OS_WIN
    static QSettings AppSettings(QSettings::IniFormat, QSettings::UserScope, AppOrg, AppName.toLower());
#else
    static QSettings AppSettings(QSettings::NativeFormat, QSettings::UserScope, AppOrg, AppName.toLower());
#endif

#endif // GLOBAL_H
