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

#include "dvdrwtoolsdependencyfactory.h"

#include <QFile>
#include <QSharedPointer>
#include <QString>

class DvdrwtoolsToolDependencyFactory : public DvdrwtoolsDependencyFactory {
public:
    DvdrwtoolsToolDependencyFactory(const QString& new_tool_name);
    const QStringList showToolDetectionInformations() override;
    bool toolExistsInSystem() override;
    const QString& getToolVersion() override;
    const QString getFileNamePath() override;

private:
    const QString tool_name;
    QSharedPointer<QFile> file;
    const QString version;
};
