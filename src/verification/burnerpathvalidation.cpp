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

#include "burnerpathvalidation.h"
#include "constants/messages.h"

BurnerPathValidation::BurnerPathValidation(QSharedPointer<BurnerWidgets> new_burner_widgets,
                                           QPointer<BurnerStage> new_start_burner_stage)
    : burner_widgets{new_burner_widgets},
      start_burner_stage{new_start_burner_stage},
      media_info_dependency{ new DvdrwtoolsToolDependencyFactory("dvd+rw-mediainfo") },
      process{nullptr},
      dvd_info{}
{

}

void BurnerPathValidation::start()
{
    if (burner_widgets.data()->lineedit_burner_path->text().isEmpty())
        return;

    burner_widgets.data()->status_bar->showMessage(Messages::reading_info);
    process = new QProcess();

    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
    connect(process, &QProcess::readyReadStandardOutput, this, &BurnerPathValidation::catchAndLogProcessOutput);
    connect(process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, qOverload<int, QProcess::ExitStatus>(&BurnerPathValidation::useProcessData));

    dvd_info.clear();
    process->start(media_info_dependency.data()->getFileNamePath(), QStringList() << burner_widgets.data()->lineedit_burner_path->text().simplified());

}

void BurnerPathValidation::fillWithBurnerData()
{
    QRegularExpression burner_line("INQUIRY:\\s*(.+)");
    QRegularExpression media_line("(?:MOUNTED MEDIA|MEDIA BOOK TYPE|MEDIA ID):\\s*(.+)");
    QRegularExpression speed_line("WRITE SPEED\\s*\\#\\d\\:\\s*((?:[0-9]|[,.])+)[xX].*");
    QString burner_txt;
    QStringList media_txt;

    for (int i = 0; i < dvd_info.size(); i++)
    {
        QRegularExpressionMatch burner_line_match = burner_line.match(dvd_info.at(i).simplified().toUpper());
        QRegularExpressionMatch media_line_match = media_line.match(dvd_info.at(i).simplified().toUpper());
        QRegularExpressionMatch speed_line_match = speed_line.match(dvd_info.at(i).simplified().toUpper());

        if (burner_line_match.hasMatch())
            burner_txt = burner_line_match.captured(0).simplified();

        if (media_line_match.hasMatch())
            media_txt.append(media_line_match.captured(0).simplified().split(","));

        if (speed_line_match.hasMatch())
            burner_widgets.data()->combo_box_write_speed->addItem(QIcon(":/images/cdrom.png"), speed_line_match.captured(0).simplified());
    }

    if (burner_txt.simplified().isEmpty())
        burner_txt = tr("Not available!");

    if (media_txt.filter(QRegularExpression(".*\\S.*")).isEmpty())
        media_txt.append(tr("Not available!"));

    burner_widgets.data()->status_bar->showMessage(tr("Ready."));
    burner_widgets.data()->label_info->setText(Messages::burnerMediaAvailability(burner_txt, media_txt));
}

void BurnerPathValidation::clearData()
{
    burner_widgets.data()->combo_box_write_speed->clear();
    burner_widgets.data()->label_info->clear();
    delete process;
}

void BurnerPathValidation::log(const int exit_code,
                               const QProcess::ExitStatus exit_status)
{
    QString errorMessage = Messages::burningErrorMessage(start_burner_stage->getProcess()->errorString(), exit_code, exit_status);
    burner_widgets.data()->plain_text_edit_with_logs->appendPlainText(errorMessage);
    burner_widgets.data()->status_bar->showMessage(errorMessage);
}

void BurnerPathValidation::catchAndLogProcessOutput()
{
    QString media_process_data = process->readAllStandardOutput().data();
    burner_widgets.data()->plain_text_edit_with_logs->appendPlainText(media_process_data);
    dvd_info.append(media_process_data.split("\n"));
}

void BurnerPathValidation::useProcessData(const int exit_code,
                                          const QProcess::ExitStatus exit_status)
{
    clearData();

    if (exit_code < 0 || exit_status != 0)
    {
        log(exit_code, exit_status);
        return;
    }

    fillWithBurnerData();
}
