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

#include "applicationinformations.h"

#include <QCoreApplication>

ApplicationInformations::ApplicationInformations()
{
    QCoreApplication::setOrganizationDomain("https://github.com/juliagoda/XboxBurner");
    QCoreApplication::setOrganizationName("juliagoda");
    QCoreApplication::setApplicationName("XBoxBurner");
    QCoreApplication::setApplicationVersion("1.4");
}

const QString ApplicationInformations::getApplicationName() const
{
    return QCoreApplication::applicationName();
}

const QString ApplicationInformations::getOrganizationDomain() const
{
    return QCoreApplication::organizationDomain();
}

const QString ApplicationInformations::getOrganizationName() const
{
    return QCoreApplication::organizationName();
}

const QString ApplicationInformations::getApplicationVersion() const
{
    return QCoreApplication::applicationVersion();
}

const QString ApplicationInformations::getApplicationFullName() const
{
    return QCoreApplication::applicationName().append(" ").append(QCoreApplication::applicationVersion());
}

QSettings::Format ApplicationInformations::getApplicationStorageFormat() const
{
#ifdef Q_OS_WIN
    return QSettings::IniFormat;
#else
    return QSettings::NativeFormat;
#endif
}
