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

#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QToolBar>
#include <QProgressBar>
#include <QFutureWatcher>
#include <QStatusBar>

struct BurnerWidgets
{
    QComboBox* combo_box_dvd_format = nullptr;
    QCheckBox* check_box_dao_mode = nullptr;
    QCheckBox* check_box_dvd_compat = nullptr;
    QCheckBox* check_box_dry_run = nullptr;
    QComboBox* combo_box_write_speed = nullptr;
    QLineEdit* lineedit_burner_path = nullptr;
    QLineEdit* lineedit_image_path = nullptr;

    QPlainTextEdit* plain_text_edit_with_logs = nullptr;
    QToolBar* toolbar = nullptr;
    QProgressBar* progress_bar_burn = nullptr;
    QProgressBar* progress_bar_ring_buffer_unit = nullptr;
    QProgressBar* progress_bar_unit_buffer_unit = nullptr;

    QFutureWatcher<bool>* backup_future_watcher = nullptr;
    QCheckBox* check_box_backup_creation = nullptr;
    QStatusBar* status_bar = nullptr;
};

struct BurnerProgressBarsWidgets
{
    QProgressBar* progress_bar_burn = nullptr;
    QProgressBar* progress_bar_ring_buffer_unit = nullptr;
    QProgressBar* progress_bar_unit_buffer_unit = nullptr;
};
