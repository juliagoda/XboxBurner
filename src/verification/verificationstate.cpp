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

#include "verificationstate.h"
#include "verification.h"

VerificationState::VerificationState(Verification* new_verification) :
    verification(new_verification),
    burner_progress_bars_setup{ QSharedPointer<BurnerProgressBarsSetup>(new BurnerProgressBarsSetup(verification->getBurnerWidgets())) }
{}

void VerificationState::showCancelMessage(const QString &message)
{
    verification->getBurnerWidgets()->status_bar->showMessage(message, 0);
    verification->getBurnerWidgets()->plain_text_edit_with_logs->appendPlainText(message);
    verification->getBurnerWidgets()->toolbar->actions().at(4)->setIcon(QIcon(":/images/start.png"));
    verification->getBurnerWidgets()->toolbar->actions().at(4)->setText(QObject::tr("&Start again"));

    burner_progress_bars_setup.data()->resetRingAndUnitProgressBarsValues();
}
