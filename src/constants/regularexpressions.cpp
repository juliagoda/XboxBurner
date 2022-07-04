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

#include "regularexpressions.h"

const QRegularExpression RegularExpressions::burnerLine()
{
    return QRegularExpression("INQUIRY:\\s*(.+)");
}

const QRegularExpression RegularExpressions::mediaLine()
{
    return QRegularExpression("(?:MOUNTED MEDIA|MEDIA BOOK TYPE|MEDIA ID):\\s*(.+)");
}

const QRegularExpression RegularExpressions::speedLine()
{
    return QRegularExpression("WRITE SPEED\\s*\\#\\d\\:\\s*((?:[0-9]|[,.])+)[xX].*");
}

const QRegularExpression RegularExpressions::progressLine()
{
    return QRegularExpression("\\d+/\\d+\\s*\\(\\s*((?:[0-9]|[.,])+)\\%\\s*\\)\\s*@((?:[0-9]|[.,])+)[xX],\\s+\\w+\\s+((?:[0-9]|[:?])+)\\s+\\w+\\s+((?:[0-9]|[.,])+)\\%\\s+\\w+\\s+((?:[0-9]|[.,])+).*");
}
