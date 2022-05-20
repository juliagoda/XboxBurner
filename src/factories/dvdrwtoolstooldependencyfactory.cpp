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

#include "dvdrwtoolstooldependencyfactory.h"

#include <QObject>
#include <QProcess>
#include <QStringList>

DvdrwtoolsToolDependencyFactory::DvdrwtoolsToolDependencyFactory(const QString& new_tool_name)
    : tool_name { new_tool_name }
    , file { new QFile(getFileFullName(new_tool_name)) }
    , version { getVersion(file->fileName()) }

{
}

const QStringList DvdrwtoolsToolDependencyFactory::showToolDetectionInformations()
{
    if (toolExistsInSystem())
        return QStringList() << QObject::tr("Info: %1 found in %2\n").arg(tool_name).arg(file.data()->fileName().mid(0, file.data()->fileName().lastIndexOf("/"))) << getToolVersion();

    return QStringList() << QObject::tr("Error: %1 not found!").arg(tool_name);
}

bool DvdrwtoolsToolDependencyFactory::toolExistsInSystem()
{
    return file.data()->exists();
}

const QString& DvdrwtoolsToolDependencyFactory::getToolVersion()
{
    return version;
}

const QString DvdrwtoolsToolDependencyFactory::getFileNamePath()
{
    return file.data()->fileName();
}
