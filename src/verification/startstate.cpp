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

#include "startstate.h"
#include "cancelstate.h"
#include "verification.h"
#include "imageverificationstate.h"
#include "constants/boxmessages.h"
#include "constants/messages.h"

#include <QMessageBox>

StartState::StartState(Verification* new_verification) :
    VerificationState(new_verification)
{}

VerificationState::CurrentState StartState::getCurrentState() const
{
    return VerificationState::CurrentState::Started;
}

void StartState::onTrigger()
{
    if (!verification->notEmptyPaths() && !verification->verificationChecked())
    {
        QMessageBox::warning(new QWidget, QObject::tr("Verification launch failure"), QObject::tr("Are you sure that the verification option is checked or the paths for the image and burning are specified?"), QMessageBox::Cancel, QMessageBox::Cancel);
        return;
    }

    verification->changeState(QSharedPointer<ImageVerificationState>(new ImageVerificationState(verification)));
    verification->prepareWidgetsBeforeCalculations();
}

void StartState::onCancel()
{
    verification->changeState(QSharedPointer<CancelState>(new CancelState(verification)));
    showCancelMessage(QObject::tr("Cancelled start"));
}

void StartState::onPrepareWidgetsBeforeCalculations()
{
    // it's start state, so we start again from beginning
     verification->trigger();
}

QString StartState::onCalculateMd5()
{
   // it's start state, so we start again from beginning
    verification->trigger();
}

void StartState::onCalculateMd5Hash()
{
  // it's start state, so we start again from beginning
    verification->trigger();
}
