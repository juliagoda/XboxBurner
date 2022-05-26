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

#include "burnerprogressbarssetup.h"
#include "burnerwidgets.h"

#include <QString>
#include <QProcess>


class BurnerStage : public QObject
{
    Q_OBJECT

public:
    BurnerStage(QSharedPointer<BurnerWidgets> new_burner_widgets) :
        burner_widgets{new_burner_widgets},
        burner_progress_bars_setup{
            QSharedPointer<BurnerProgressBarsSetup>(
            new BurnerProgressBarsSetup(new_burner_widgets))},
        next_stage{nullptr},
        burn_process{nullptr},
        burning{false}
    {}

    BurnerStage* next(BurnerStage* next_burner_stage)
    {
        next_stage = next_burner_stage;
        return next_stage;
    }

    virtual bool handle()
    {
        if (next_stage)
            return next_stage->handle();

        return true;
    }

protected:
    QSharedPointer<BurnerWidgets> burner_widgets;
    QSharedPointer<BurnerProgressBarsSetup> burner_progress_bars_setup;
    BurnerStage* next_stage;
    QProcess* burn_process;
    bool burning;
};
