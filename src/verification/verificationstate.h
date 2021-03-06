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

#include "burner/burnerwidgets.h"
#include "burner/burnerprogressbarssetup.h"

#include <QString>

class Verification;

class VerificationState
{
public:

    enum class CurrentState
    {
        Cancelled,
        Started,
        VerificatedImage,
        VerificatedDvd
    };

    VerificationState(Verification* new_verification);
    virtual ~VerificationState() = default;
    virtual CurrentState getCurrentState() const = 0;
    virtual void onTrigger() = 0;
    virtual void onCancel() = 0;
    virtual void onPrepareWidgetsBeforeCalculations() = 0;
    virtual QString onCalculateMd5() = 0;
    virtual void onCalculateMd5Hash() = 0;

protected:
    Verification* verification;
    QSharedPointer<BurnerProgressBarsSetup> burner_progress_bars_setup;

    void showCancelMessage(const QString& message);
};
