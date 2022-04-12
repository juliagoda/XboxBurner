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

#include "messages.h"

#include <QLocale>
#include <QObject>
#include <QRegularExpression>

const QString Messages::burnerMediaAvailability(const QString& burner_txt, const QStringList& media_txt_list)
{
    return QObject::tr("Burner: <font color=grey>%1</font><br>Media: <font color=grey>%2</font>").arg(burner_txt).arg(media_txt_list.filter(QRegularExpression(".*\\S.*")).join(",").simplified().replace(", ", ","));
}

const QString Messages::burningErrorMessage(const QString& burn_process_error_message, int exit_code, int exit_status)
{
    return QObject::tr("Error: %1 (%2, %3)").arg(burn_process_error_message).arg(QString::number(exit_code)).arg(QString::number(exit_status));
}

const QString Messages::imageFromImagePath(const QString& image_path)
{
    return image_path.simplified().mid(image_path.simplified().lastIndexOf("/") + 1, image_path.simplified().length() - image_path.simplified().lastIndexOf("/"));
}

const QString Messages::backupCreationDestination(const QString& image_path)
{
    return image_path.mid(0, image_path.lastIndexOf(".")) + "_backup" + image_path.mid(image_path.lastIndexOf("."), image_path.length() - image_path.lastIndexOf("."));
}

const QString Messages::reading_info = QObject::tr("Reading burner and media info...");

const QString Messages::burn_process_start_with_date = QObject::tr("Burn process started at %1").arg(QLocale().dateFormat(QLocale::ShortFormat));

const QString Messages::backup_creation = QObject::tr("Creating backup of game image...");

const QString Messages::checksum_calculation_image = QObject::tr("Calculating checksum of image...");

const QString Messages::checksum_calculation_dvd = QObject::tr("Calculating checksum of DVD...");
