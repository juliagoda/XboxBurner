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
#include "burner/burnerstartstage.h"
#include "dependencies/dvdrwtoolstooldependencyfactory.h"

#include <QObject>
#include <QProcess>
#include <QPointer>
#include <QStringList>

class BurnerPathValidation : public QObject
{
    Q_OBJECT

public:
    explicit BurnerPathValidation(QSharedPointer<BurnerWidgets> new_burner_widgets,
                                  QPointer<BurnerStage> new_start_burner_stage);
    void start();

protected:
    void fillWithBurnerData();
    void clearData();
    void log(const int exit_code,
             const QProcess::ExitStatus exit_status);

private slots:
    void catchAndLogProcessOutput();
    void useProcessData(const int exit_code,
                        const QProcess::ExitStatus exit_status);

private:
   QSharedPointer<BurnerWidgets> burner_widgets;
   QPointer<BurnerStage> start_burner_stage;
   QSharedPointer<DvdrwtoolsToolDependencyFactory> media_info_dependency;
   QProcess* process;
   QStringList dvd_info;
};

