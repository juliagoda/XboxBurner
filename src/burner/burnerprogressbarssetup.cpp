#include "burnerprogressbarssetup.h"

BurnerProgressBarsSetup::BurnerProgressBarsSetup(const BurnerProgressBarsWidgets& new_burner_progress_bars_widgets) :
    burner_progress_bars_widgets(new_burner_progress_bars_widgets)
{

}

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
