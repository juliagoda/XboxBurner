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

#include "pathsandspeedcontentverificationstage.h"

#include "constants/boxmessages.h"

PathsAndSpeedContentVerificationStage::PathsAndSpeedContentVerificationStage(QSharedPointer<BurnerWidgets> new_burner_widgets) :
    BurnerStage(new_burner_widgets)
{}

bool PathsAndSpeedContentVerificationStage::handle()
{
    if (burner_widgets.data()->lineedit_image_path->text().isEmpty() ||
            burner_widgets.data()->lineedit_burner_path->text().isEmpty() ||
            burner_widgets.data()->combo_box_write_speed->currentText().isEmpty())
        return false;

    return BurnerStage::handle();
}
