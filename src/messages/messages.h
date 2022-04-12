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

#include <QString>

class Messages {
public:
    static const QString burnerMediaAvailability(const QString& burner_txt, const QStringList& media_txt_list);
    static const QString burningErrorMessage(const QString& burn_process_error_message, int exit_code, int exit_status);
    static const QString imageFromImagePath(const QString& image_path);
    static const QString backupCreationDestination(const QString& image_path);

    static const QString reading_info;
    static const QString burn_process_start_with_date;
    static const QString backup_creation;
    static const QString checksum_calculation_image;
    static const QString checksum_calculation_dvd;
};
