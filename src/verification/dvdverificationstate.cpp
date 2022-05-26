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

#include "dvdverificationstate.h"
#include "verification.h"
#include "constants/boxmessages.h"
#include "constants/messages.h"
#include "cancelstate.h"

#include <QtConcurrent>

DvdVerificationState::DvdVerificationState(Verification* new_verification) :
    VerificationState(new_verification),
    burner_progress_bars_setup{ QSharedPointer<BurnerProgressBarsSetup>(new BurnerProgressBarsSetup(verification->getBurnerWidgets())) }
{
    QObject::connect(verification->getBurnerWidgets()->dvd_future_watcher, &QFutureWatcher<QString>::finished, verification, &Verification::calculateMd5Hash);
}

void DvdVerificationState::onTrigger()
{
   verification->changeState(QSharedPointer<CancelState>(new CancelState(verification)));
   verification->trigger();
}

void DvdVerificationState::onPrepareWidgetsBeforeCalculations()
{
    verification->getBurnerWidgets()->toolbar->actions().at(4)->setIcon(QIcon(":/images/cancel.png"));
    verification->getBurnerWidgets()->toolbar->actions().at(4)->setText(QObject::tr("&Cancel verification"));

    burner_progress_bars_setup.data()->resetRingAndUnitProgressBarsValues();

    verification->getBurnerWidgets()->status_bar->showMessage(Messages::checksum_calculation_dvd);
    verification->getBurnerWidgets()->plain_text_edit_with_logs->appendPlainText("Checksum of image already exists.");
    verification->getBurnerWidgets()->plain_text_edit_with_logs->appendPlainText(Messages::checksum_calculation_dvd);
    verification->getBurnerWidgets()->progress_bar_burn->setValue(0);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QFuture<QString> dvdFuture = QtConcurrent::run(this, &DvdVerificationState::onCalculateMd5);
#else
    QFuture<QString> dvdFuture = QtConcurrent::run(&DvdVerificationState::onCalculateMd5, this);
#endif
    verification->getBurnerWidgets()->dvd_future_watcher->setFuture(dvdFuture);
}

QString DvdVerificationState::onCalculateMd5()
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    QFile device(verification->getBurnerWidgets()->lineedit_burner_path->text().simplified());
    QFile image(verification->getBurnerWidgets()->lineedit_image_path->text().simplified());
    int block_count = image.size() / verification->getBlockSize();
    verification->setMaximalPossibleMD5HashValue(block_count);

    if (device.open(QFile::ReadOnly))
    {
        char buf[verification->getBlockSize()];

        for (int block_number = 0; block_number < block_count; block_number++)
        {
            device.read(buf, verification->getBlockSize());
            hash.addData(buf, verification->getBlockSize());
            verification->calculateMD5HashForProgressBar(block_number + 1);
        }

        device.close();
        return hash.result().toHex();
    }

    return QString();
}

void DvdVerificationState::onCalculateMd5Hash()
{
    verification->getBurnerWidgets()->toolbar->actions().at(4)->setIcon(QIcon(":/images/verify.png"));
    verification->getBurnerWidgets()->toolbar->actions().at(4)->setText(QObject::tr("&Verify"));
    burner_progress_bars_setup.data()->restoreRingAndUnitProgressBarsValues();
    QString dvd_checksum = verification->getBurnerWidgets()->dvd_future_watcher->result();

    if (dvd_checksum.isEmpty())
    {
        verification->getBurnerWidgets()->status_bar->showMessage(Messages::checksum_calculation_dvd_fail);
        verification->getBurnerWidgets()->plain_text_edit_with_logs->appendPlainText(Messages::checksum_calculation_dvd_fail);
        verification->changeState(QSharedPointer<CancelState>(new CancelState(verification)));
        return;
    }

    verification->getBurnerWidgets()->plain_text_edit_with_logs->appendPlainText("Checksum of DVD: " + dvd_checksum);
    QString background_color = QString("green");
    QString checksum_message = Messages::checksum_with_success;

    if (verification->getImageChecksum() != dvd_checksum)
    {
        background_color = QString("red");
        checksum_message = Messages::checksum_fail;
    }

    verification->getBurnerWidgets()->status_frame->setStyleSheet(QString("QFrame { background-color : %1; }").arg(background_color));
    verification->getBurnerWidgets()->status_bar->showMessage(checksum_message);
    verification->getBurnerWidgets()->plain_text_edit_with_logs->appendPlainText(checksum_message);
}
