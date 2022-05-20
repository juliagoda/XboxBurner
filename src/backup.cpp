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

#include "backup.h"

#include "burner/burnerstartstage.h"
#include "constants/boxmessages.h"
#include "constants/messages.h"

#include <QString>
#include <QFile>

Backup::Backup(const BurnerWidgets& new_burner_widgets) :
    burner_widgets(new_burner_widgets),
    burner_progress_bars_setup({new_burner_widgets.progress_bar_burn,
                               new_burner_widgets.progress_bar_ring_buffer_unit,
                               new_burner_widgets.progress_bar_unit_buffer_unit})
{}

bool Backup::create()
{
    QString file_name = burner_widgets.lineedit_image_path->text().simplified();
    QString source = file_name;
    QString destination = Messages::backupCreationDestination(file_name);

    return QFile::copy(source, destination);
}

void Backup::log()
{
    bool result = burner_widgets.backup_future_watcher->result();
    burner_progress_bars_setup.restoreRingAndUnitProgressBarsValues();

    if (result)
    {
        burner_widgets.status_bar->showMessage(QObject::tr("Creation backup of game image successfully!"), 0);
        burner_widgets.plain_text_edit_with_logs->appendPlainText("Creation backup of game image successfully!");
        resizeImage();
    }
    else
    {
        burner_widgets.status_bar->showMessage(QObject::tr("Creation backup of game image failed!"), 0);
        burner_widgets.plain_text_edit_with_logs->appendPlainText("Creation backup of game image failed! Burn process stopped!");
    }
}

void Backup::resizeImage()
{
    qint64 size = Q_INT64_C(8547991552);
    QFile file(burner_widgets.lineedit_image_path->text().simplified());

    if (file.resize(size))
    {
        QSharedPointer<BurnerStartStage> burner_start_stage = QSharedPointer<BurnerStartStage>(new BurnerStartStage(burner_widgets));
        burner_start_stage->handle();
    }
    else
    {
        burner_widgets.status_bar->showMessage(QObject::tr("Resize game image failed!"), 0);
        burner_widgets.plain_text_edit_with_logs->appendPlainText("Resize game image failed! Burn process stopped!");
    }
}
