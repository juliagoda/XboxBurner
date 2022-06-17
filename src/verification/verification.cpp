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

#include "verification.h"
#include "startstate.h"
#include "cancelstate.h"
#include "imageverificationstate.h"
#include "dvdverificationstate.h"
#include "constants/boxmessages.h"
#include "constants/messages.h"
#include "burner/burnerprogressbarssetup.h"

#include <QtConcurrent>

Verification::Verification(QSharedPointer<BurnerWidgets> new_burner_widgets) :
    burner_widgets{new_burner_widgets},
    verification_state{QSharedPointer<StartState>(new StartState(this))},
    image_checksum{QString()},
    block_size{2048},
    verification_progress_max{0}
{}

void Verification::trigger()
{
    verification_state->onTrigger();
}

void Verification::cancel()
{
    verification_state->onCancel();
}

void Verification::prepareWidgetsBeforeCalculations()
{
    verification_state->onPrepareWidgetsBeforeCalculations();
}

void Verification::calculateMd5()
{
    verification_state->onCalculateMd5();
}

void Verification::calculateMd5Hash()
{
   verification_state->onCalculateMd5Hash();
}

bool Verification::notEmptyPaths()
{
    return (!burner_widgets.data()->lineedit_image_path->text().isEmpty() &&
            !burner_widgets.data()->lineedit_burner_path->text().isEmpty());
}

bool Verification::verificationChecked()
{
    return burner_widgets.data()->check_box_data_verification->isChecked();
}

QSharedPointer<VerificationState> Verification::getState() const
{
    return verification_state;
}

void Verification::changeState(QSharedPointer<VerificationState> state)
{
    verification_state = state;
}

VerificationState::CurrentState Verification::getCurrentState() const
{
    return verification_state->getCurrentState();
}

void Verification::calculateMD5HashForProgressBar(qint64 hash_current_block)
{
    burner_widgets.data()->progress_bar_burn->setValue(hash_current_block * 100 / verification_progress_max);
}

void Verification::setMaximalPossibleMD5HashValue(qint64 maximal_value)
{
    verification_progress_max = maximal_value;
}

QSharedPointer<BurnerWidgets> Verification::getBurnerWidgets() const
{
    return burner_widgets;
}

int Verification::getBlockSize() const
{
    return block_size;
}

QString Verification::getImageChecksum() const
{
    return image_checksum;
}

void Verification::updateMd5Checksum(const QString& new_image_checksum)
{
    image_checksum = new_image_checksum;
}
