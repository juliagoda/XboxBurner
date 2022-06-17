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
    QObject::connect(burn_process, SIGNAL(readyReadStandardOutput()), this, SLOT(burnProcess_readyReadStandardOutput()));
    QObject::connect(burn_process, SIGNAL(readyReadStandardError()), this, SLOT(burnProcess_readyReadStandardOutput()));
    QObject::connect(burn_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(burnProcess_finished(int, QProcess::ExitStatus)));

    QStringList arguments;
    QString layerbreak = layerBreak();

    if (!layerbreak.isEmpty())
        arguments.append("-use-the-force-luke=break:" + layerbreak);

    if (burner_widgets.data()->combo_box_dvd_format->currentIndex() != 4)
    {
        if (burner_widgets.data()->check_box_dao_mode->isChecked())
            arguments.append("-use-the-force-luke=dao");

        if (burner_widgets.data()->check_box_dvd_compat->isChecked())
            arguments.append("-dvd-compat");
    }

    if (burner_widgets.data()->check_box_dry_run->isChecked())
        arguments.append("-dry-run");

    arguments.append("-speed=" + burner_widgets.data()->combo_box_write_speed->currentText().simplified());
    arguments.append("-Z");
    arguments.append(burner_widgets.data()->lineedit_burner_path->text().simplified() + "=" + burner_widgets.data()->lineedit_image_path->text().simplified());

    burn_process->setProcessChannelMode(QProcess::MergedChannels);
    QScopedPointer<DvdrwtoolsToolDependencyFactory> growisofs { new DvdrwtoolsToolDependencyFactory("growisofs") };
    burn_process->start(growisofs.data()->getFileNamePath(), arguments);

    return BurnerStage::handle();
}

QString BurnerStartStage::layerBreak()
{
    QString result("");

    switch (burner_widgets.data()->combo_box_dvd_format->currentIndex())
    {
    case 0:
        result = "";
        break;
    case 1:
        result = "1913776";
        break;
    case 2:
        result = "1913760";
        break;
    case 3:
        result = "2086912";
        break;
    case 4:
        result = "2133520";
        break;
    case 5:
        QString tmp = burner_widgets.data()->combo_box_dvd_format->itemText(5);
        result = tmp.mid(tmp.indexOf("(") + 1, tmp.indexOf(")") - tmp.indexOf("(") - 1);
        break;
    }

    return result;
}

void BurnerStartStage::burnProcess_readyReadStandardOutput()
{
    int burnProgress, rbuProgress, ubuProgress;
    QString growisofsData;
    QRegularExpression progressLine("\\d+/\\d+\\s*\\(\\s*((?:[0-9]|[.,])+)\\%\\s*\\)\\s*@((?:[0-9]|[.,])+)[xX],\\s+\\w+\\s+((?:[0-9]|[:?])+)\\s+\\w+\\s+((?:[0-9]|[.,])+)\\%\\s+\\w+\\s+((?:[0-9]|[.,])+).*");

    while (burn_process->canReadLine())
    {
        growisofsData = burn_process->readAll().simplified().data();

        if (!growisofsData.isEmpty())
            burner_widgets.data()->plain_text_edit_with_logs->appendPlainText(growisofsData);

        if (growisofsData.contains("flushing cache"))
        {
            burner_progress_bars_setup.data()->setValuesProgressBarsToZero();
            burner_progress_bars_setup.data()->resetBurnerProgressBarValues();
            burner_widgets.data()->status_bar->showMessage(QObject::tr("Fluching cache..."));
        }
        else if (growisofsData.contains("closing track"))
        {
            burner_widgets.data()->status_bar->showMessage(QObject::tr("Closing track..."));
        }
        else if (growisofsData.contains("closing disc"))
        {
            burner_widgets.data()->status_bar->showMessage(QObject::tr("Closing disc..."));
        }
        else if (growisofsData.contains("reloading tray"))
        {
            burner_widgets.data()->status_bar->showMessage(QObject::tr("Reloading tray..."));
        }
        else if (!growisofsData.isEmpty())
        {
            QRegularExpressionMatch progress_line_match = progressLine.match(growisofsData);

            if (progress_line_match.hasMatch())
            {
                burnProgress = progress_line_match.captured(0).simplified().toFloat();
                rbuProgress = progress_line_match.captured(3).simplified().toFloat();
                ubuProgress = progress_line_match.captured(4).simplified().toFloat();

                if (burnProgress > 0 && burnProgress <= 100)
                    burner_widgets.data()->progress_bar_burn->setValue(burnProgress);

                if (rbuProgress > 0 && rbuProgress <= 100)
                    burner_widgets.data()->progress_bar_ring_buffer_unit->setValue(rbuProgress);

                if (ubuProgress > 0 && ubuProgress <= 100)
                    burner_widgets.data()->progress_bar_unit_buffer_unit->setValue(ubuProgress);

                const QString image = Messages::imageFromImagePath(burner_widgets.data()->lineedit_image_path->text());
                const QString drive = burner_widgets.data()->lineedit_burner_path->text().simplified();
                const QString speed = progress_line_match.captured(1).simplified();
                const QString eta = progress_line_match.captured(2).simplified();

                if (eta == "??:??")
                    burner_widgets.data()->status_bar->showMessage(QObject::tr("Preparing burn process..."));
                else
                    burner_widgets.data()->status_bar->showMessage(QObject::tr("Burning '%1' on %2 at %3x -> ETA %4").arg(image, drive, speed, eta));
            }
        }
    }
}

void BurnerStartStage::burnProcess_finished(const int exitCode, const QProcess::ExitStatus exitStatus)
{
    burner_progress_bars_setup.data()->restoreBurnerProgressBarValues();

    if (exitCode == 0 && exitStatus == 1)
    {
        burner_progress_bars_setup.data()->setValuesProgressBarsToZero();
        burner_widgets.data()->status_bar->showMessage(QObject::tr("Burn process canceled."));
    }
    else if (exitCode == 0 && exitStatus == 0)
    {
        burner_widgets.data()->progress_bar_burn->setValue(100);
        burner_widgets.data()->status_bar->showMessage(QObject::tr("Burn process successfully."));
    }
    else
    {
        QString errorMessage = Messages::burningErrorMessage(burn_process->errorString(), exitCode, exitStatus);
        burner_widgets.data()->plain_text_edit_with_logs->appendPlainText(errorMessage);
        burner_widgets.data()->status_bar->showMessage(errorMessage);
    }

    burner_widgets.data()->toolbar->actions().at(3)->setIcon(QIcon(":/images/burn.png"));
    burner_widgets.data()->toolbar->actions().at(3)->setText(QObject::tr("&Burn"));

    burning = false;
    delete burn_process;

    emit finishedBurning();
}
