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

#include "burningacceptancestage.h"

#include "constants/boxmessages.h"
#include "constants/messages.h"

#include <QMessageBox>

BurningAcceptanceStage::BurningAcceptanceStage(QSharedPointer<BurnerWidgets> new_burner_widgets) :
    BurnerStage(new_burner_widgets)
{}

bool BurningAcceptanceStage::handle()
{
    if (BoxMessages::startBurnProcessMessage(new QWidget) != QMessageBox::Ok)
        return false;

    burner_widgets.data()->plain_text_edit_with_logs->appendPlainText(Messages::burn_process_start_with_date);
    burner_widgets.data()->toolbar->actions().at(3)->setIcon(QIcon(":/images/cancel.png"));
    burner_widgets.data()->toolbar->actions().at(3)->setText(QObject::tr("&Cancel"));
    burner_progress_bars_setup.data()->setValuesProgressBarsToZero();

    return BurnerStage::handle();
}
