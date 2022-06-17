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

#include "cancelstate.h"
#include "startstate.h"
#include "verification.h"
#include "constants/boxmessages.h"
#include "constants/messages.h"

CancelState::CancelState(Verification* new_verification) :
    VerificationState(new_verification)
{}

VerificationState::CurrentState CancelState::getCurrentState() const
{
   return VerificationState::CurrentState::Cancelled;
}

void CancelState::onTrigger()
{
    verification->changeState(QSharedPointer<StartState>(new StartState(verification)));
    verification->trigger();
}

void CancelState::onCancel()
{
   showCancelMessage(QObject::tr("It's already cancelled!"));
}

void CancelState::onPrepareWidgetsBeforeCalculations()
{
    showCancelMessage(QObject::tr("Widgets cannot be prepared. Verification cancelled"));
}

QString CancelState::onCalculateMd5()
{
    showCancelMessage(QObject::tr("Md5 hash cannot be calculated. Verification cancelled"));
    return QString();
}

void CancelState::onCalculateMd5Hash()
{
    showCancelMessage(QObject::tr("Md5 hash calculation cannot be finished. Verification cancelled"));
}
