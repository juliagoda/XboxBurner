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

#include "mediainfodependencyfactory.h"

MediaInfoDependencyFactory::MediaInfoDependencyFactory()
    : tool_name { "dvd+rw-mediainfo" }
    , file { new QFile(getFileFullName("dvd+rw-mediainfo")) }
    , version { getVersion(file.fileName()) }
{
}

QStringList& MediaInfoDependencyFactory::showToolDetectionInformations()
{
    if (toolExistsInSystem()) {
        return QStringList() << QObject::tr("Info: dvd+rw-mediainfo found in %1").arg(file.data()->fileName().mid(0, file.data()->fileName().lastIndexOf("/"))) << getToolVersion();
    }

    return QStringList() << QObject::tr("Error: dvd+rw-mediainfo not found!");
}

bool MediaInfoDependencyFactory::toolExistsInSystem()
{
    return file.exists();
}

const QString MediaInfoDependencyFactory::getToolVersion() const
{
    return version;
}
