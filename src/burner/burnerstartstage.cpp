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

#include "burnerstartstage.h"

#include "constants/boxmessages.h"
#include "constants/messages.h"
#include "dependencies/dvdrwtoolstooldependencyfactory.h"

#include <QObject>
#include <QtGlobal>

BurnerStartStage::BurnerStartStage(QSharedPointer<BurnerWidgets> new_burner_widgets) :
    BurnerStage(new_burner_widgets)
{}

bool BurnerStartStage::handle()
{
    burner_widgets.data()->status_bar->showMessage(QObject::tr("Starting burn process..."));
    burning = true;
    burn_process = new QProcess();

    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
    QObject::connect(burn_process, &QProcess::readyReadStandardOutput, this, &BurnerStartStage::burnProcess_readyReadStandardOutput);
    QObject::connect(burn_process, &QProcess::readyReadStandardError, this, &BurnerStartStage::burnProcess_readyReadStandardOutput);
    QObject::connect(burn_process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, qOverload<int, QProcess::ExitStatus>(&BurnerStartStage::burnProcess_finished));

    QStringList process_arguments;

    if (!layerBreak().isEmpty())
        process_arguments.append("-use-the-force-luke=break:" + layerBreak());

    if (burner_widgets.data()->combo_box_dvd_format->currentIndex() != 4)
    {
        if (burner_widgets.data()->check_box_dao_mode->isChecked())
            process_arguments.append("-use-the-force-luke=dao");

        if (burner_widgets.data()->check_box_dvd_compat->isChecked())
            process_arguments.append("-dvd-compat");
    }

    if (burner_widgets.data()->check_box_dry_run->isChecked())
        process_arguments.append("-dry-run");

    process_arguments.append("-speed=" + burner_widgets.data()->combo_box_write_speed->currentText().simplified());
    process_arguments.append("-Z");
    process_arguments.append(burner_widgets.data()->lineedit_burner_path->text().simplified() + "=" + burner_widgets.data()->lineedit_image_path->text().simplified());

    burn_process->setProcessChannelMode(QProcess::MergedChannels);
    QScopedPointer<DvdrwtoolsToolDependencyFactory> growisofs { new DvdrwtoolsToolDependencyFactory("growisofs") };
    burn_process->start(growisofs.data()->getFileNamePath(), process_arguments);

    return BurnerStage::handle();
}

const QString BurnerStartStage::layerBreak()
{
    switch (burner_widgets.data()->combo_box_dvd_format->currentIndex())
    {
    case 0:
        return QString();
        break;
    case 1:
        return QString("1913776");
        break;
    case 2:
        return QString("1913760");
        break;
    case 3:
        return QString("2086912");
        break;
    case 4:
        return QString("2133520");
        break;
    case 5:
        QString dvd_r_format = burner_widgets.data()->combo_box_dvd_format->itemText(5);
        return dvd_r_format.mid(dvd_r_format.indexOf("(") + 1, dvd_r_format.indexOf(")") - dvd_r_format.indexOf("(") - 1);
        break;
    }

    return QString();
}

void BurnerStartStage::burnProcess_readyReadStandardOutput()
{
    QString growisofs_data;
    QRegularExpression progress_line("\\d+/\\d+\\s*\\(\\s*((?:[0-9]|[.,])+)\\%\\s*\\)\\s*@((?:[0-9]|[.,])+)[xX],\\s+\\w+\\s+((?:[0-9]|[:?])+)\\s+\\w+\\s+((?:[0-9]|[.,])+)\\%\\s+\\w+\\s+((?:[0-9]|[.,])+).*");

    while (burn_process->canReadLine())
    {
        growisofs_data = burn_process->readAll().simplified().data();

        if (growisofs_data.isEmpty())
            return;

        updateWidgetsWithGrowisoData(growisofs_data);

        QRegularExpressionMatch progress_line_match = progress_line.match(growisofs_data);

        if (progress_line_match.hasMatch())
            updateWidgetsWithProgressData(progress_line_match);
    }
}

void BurnerStartStage::updateWidgetsWithGrowisoData(const QString& growisofs_data)
{
    if (!growisofs_data.isEmpty())
        burner_widgets.data()->plain_text_edit_with_logs->appendPlainText(growisofs_data);

    if (growisofs_data.contains("flushing cache"))
    {
        burner_progress_bars_setup.data()->setValuesProgressBarsToZero();
        burner_progress_bars_setup.data()->resetBurnerProgressBarValues();
        burner_widgets.data()->status_bar->showMessage(QObject::tr("Fluching cache..."));
    }

    if (growisofs_data.contains("closing track"))
        burner_widgets.data()->status_bar->showMessage(QObject::tr("Closing track..."));

    if (growisofs_data.contains("closing disc"))
        burner_widgets.data()->status_bar->showMessage(QObject::tr("Closing disc..."));

    if (growisofs_data.contains("reloading tray"))
        burner_widgets.data()->status_bar->showMessage(QObject::tr("Reloading tray..."));
}

void BurnerStartStage::updateWidgetsWithProgressData(QRegularExpressionMatch progress_line_match)
{
    int burn_progress = progress_line_match.captured(0).simplified().toInt();
    int rbu_progress = progress_line_match.captured(3).simplified().toInt();
    int ubu_progress = progress_line_match.captured(4).simplified().toInt();

    if (burn_progress > 0 && burn_progress <= 100)
        burner_widgets.data()->progress_bar_burn->setValue(burn_progress);

    if (rbu_progress > 0 && rbu_progress <= 100)
        burner_widgets.data()->progress_bar_ring_buffer_unit->setValue(rbu_progress);

    if (ubu_progress > 0 && ubu_progress <= 100)
        burner_widgets.data()->progress_bar_unit_buffer_unit->setValue(ubu_progress);

    const QString image = Messages::imageFromImagePath(burner_widgets.data()->lineedit_image_path->text());
    const QString drive = burner_widgets.data()->lineedit_burner_path->text().simplified();
    const QString speed = progress_line_match.captured(1).simplified();
    const QString eta = progress_line_match.captured(2).simplified();

    if (eta == "??:??")
        burner_widgets.data()->status_bar->showMessage(QObject::tr("Preparing burn process..."));
    else
        burner_widgets.data()->status_bar->showMessage(QObject::tr("Burning '%1' on %2 at %3x -> ETA %4").arg(image, drive, speed, eta));
}

void BurnerStartStage::burnProcess_finished(const int exit_code, const QProcess::ExitStatus exit_status)
{
    burner_progress_bars_setup.data()->restoreBurnerProgressBarValues();
    const bool is_process_crashed = exit_code == 0 && exit_status == 1;
    const bool is_process_successfull = exit_code == 0 && exit_status == 0;

    if (is_process_crashed)
    {
        burner_progress_bars_setup.data()->setValuesProgressBarsToZero();
        QString errorMessage = Messages::burningErrorMessage(burn_process->errorString(), exit_code, exit_status);
        burner_widgets.data()->plain_text_edit_with_logs->appendPlainText(errorMessage);
        burner_widgets.data()->status_bar->showMessage(errorMessage);
    }

    if (is_process_successfull)
    {
        burner_widgets.data()->progress_bar_burn->setValue(100);
        burner_widgets.data()->status_bar->showMessage(QObject::tr("Burn process successfully."));
    }

    burner_widgets.data()->toolbar->actions().at(3)->setIcon(QIcon(":/images/burn.png"));
    burner_widgets.data()->toolbar->actions().at(3)->setText(QObject::tr("&Burn"));

    burning = false;
    delete burn_process;

    emit finishedBurning();
}
