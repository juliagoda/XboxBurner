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

#include "burnerprogressbarssetup.h"

BurnerProgressBarsSetup::BurnerProgressBarsSetup(const BurnerProgressBarsWidgets& new_burner_progress_bars_widgets) :
    burner_progress_bars_widgets(new_burner_progress_bars_widgets)
{}

void BurnerProgressBarsSetup::setValuesProgressBarsToZero()
{
    burner_progress_bars_widgets.progress_bar_ring_buffer_unit->setValue(0);
    burner_progress_bars_widgets.progress_bar_unit_buffer_unit->setValue(0);
    burner_progress_bars_widgets.progress_bar_burn->setValue(0);
}

void BurnerProgressBarsSetup::resetBurnerProgressBarValues()
{
    burner_progress_bars_widgets.progress_bar_burn->setMinimum(0);
    burner_progress_bars_widgets.progress_bar_burn->setMaximum(0);
    burner_progress_bars_widgets.progress_bar_burn->setValue(-1);
}

void BurnerProgressBarsSetup::resetRingAndUnitProgressBarsValues()
{
    burner_progress_bars_widgets.progress_bar_ring_buffer_unit->setMinimum(0);
    burner_progress_bars_widgets.progress_bar_ring_buffer_unit->setMaximum(0);
    burner_progress_bars_widgets.progress_bar_ring_buffer_unit->setValue(-1);

    burner_progress_bars_widgets.progress_bar_unit_buffer_unit->setMinimum(0);
    burner_progress_bars_widgets.progress_bar_unit_buffer_unit->setMaximum(0);
    burner_progress_bars_widgets.progress_bar_unit_buffer_unit->setValue(-1);
}

void BurnerProgressBarsSetup::restoreBurnerProgressBarValues()
{
    burner_progress_bars_widgets.progress_bar_burn->setMinimum(0);
    burner_progress_bars_widgets.progress_bar_burn->setMaximum(100);
    burner_progress_bars_widgets.progress_bar_burn->setValue(0);
}

void BurnerProgressBarsSetup::restoreRingAndUnitProgressBarsValues()
{
    burner_progress_bars_widgets.progress_bar_ring_buffer_unit->setMinimum(0);
    burner_progress_bars_widgets.progress_bar_ring_buffer_unit->setMaximum(100);
    burner_progress_bars_widgets.progress_bar_ring_buffer_unit->setValue(0);

    burner_progress_bars_widgets.progress_bar_unit_buffer_unit->setMinimum(0);
    burner_progress_bars_widgets.progress_bar_unit_buffer_unit->setMaximum(100);
    burner_progress_bars_widgets.progress_bar_unit_buffer_unit->setValue(0);
}
