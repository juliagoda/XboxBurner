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

#include "truncateimagestartstage.h"

#include "constants/boxmessages.h"
#include "constants/messages.h"
#include "xboxburner.h"

#include <QMessageBox>
#include <QtConcurrent>

TruncateImageStartStage::TruncateImageStartStage(QSharedPointer<BurnerWidgets> new_burner_widgets,
                                                 QPointer<Backup> new_backup) :
    BurnerStage(new_burner_widgets),
    backup(new_backup)
{}

bool TruncateImageStartStage::handle()
{
    if (burner_widgets.data()->combo_box_dvd_format->currentIndex() != 3)
        return BurnerStage::handle();

    burner_widgets.data()->status_bar->showMessage(QObject::tr("Truncating XGD3 image..."));
    qint64 size = Q_INT64_C(8547991552);
    QString fileName = burner_widgets.data()->lineedit_image_path->text().simplified();
    QFile file(fileName);

    if (file.size() <= size || !burner_widgets.data()->check_box_backup_creation->isChecked())
        return BurnerStage::handle();

    burner_widgets.data()->status_bar->showMessage(Messages::backup_creation);
    burner_widgets.data()->plain_text_edit_with_logs->appendPlainText(Messages::backup_creation);
    burner_progress_bars_setup.data()->resetRingAndUnitProgressBarsValues();

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QFuture<bool> backupFuture = QtConcurrent::run(backup.data(), &Backup::create);
#else
    QFuture<bool> backupFuture = QtConcurrent::run(&Backup::create, backup.data());
#endif
    burner_widgets.data()->backup_future_watcher->setFuture(backupFuture);
}
