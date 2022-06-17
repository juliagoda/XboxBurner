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

#include "burner/burnerstage.h"
#include "verificationstate.h"

#include <QSharedPointer>

class Verification : public QObject
{
    Q_OBJECT

public:
    Verification(QSharedPointer<BurnerWidgets> new_burner_widgets);
    void trigger();
    void cancel();
    void prepareWidgetsBeforeCalculations();
    void calculateMd5();
    void calculateMd5Hash();
    bool notEmptyPaths();
    bool verificationChecked();

    QSharedPointer<VerificationState> getState() const;
    void changeState(QSharedPointer<VerificationState> state);
    VerificationState::CurrentState getCurrentState() const;
    void calculateMD5HashForProgressBar(qint64 hash_current_block);
    void setMaximalPossibleMD5HashValue(qint64 maximal_value);

    QSharedPointer<BurnerWidgets> getBurnerWidgets() const;
    int getBlockSize() const;
    QString getImageChecksum() const;

    void updateMd5Checksum(const QString& new_image_checksum);

private:
    QSharedPointer<BurnerWidgets> burner_widgets;
    QSharedPointer<VerificationState> verification_state;
    QString image_checksum;
    int block_size;
    int verification_progress_max;
};
